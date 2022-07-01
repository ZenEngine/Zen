#pragma once
#include <Core/Misc/Assert.hpp>
#include <type_traits>
#include <utility>
#include <cstddef>
#include <atomic>
#include <functional>


namespace zen
{
    template<typename T>
    struct TDefaultDelete
    {
        constexpr TDefaultDelete() noexcept = default;

        // std::is_convertible_vによってU*型からT*型に暗黙的変換が可能かどうかを判定し、
        // 可能ならtrue、不可能ならばfalseを返す。
        template<typename U>
        TDefaultDelete([[maybe_unused]] const TDefaultDelete<U>& other) noexcept
            requires(std::is_convertible_v<U*, T*>)
        {
        }

        void operator()(T* pointer) const noexcept
        {
            static_assert(sizeof(T) > 0, "incompleted type");
            delete pointer;
        }
    };

    /**
    * @brief 配列版のDefaultDelete
    */
    template<typename T>
    struct TDefaultDelete<T[]>
    {
        constexpr TDefaultDelete() noexcept = default;

        template<typename U>
        TDefaultDelete([[maybe_unused]] const TDefaultDelete<U[]>& other) noexcept
            requires (std::is_constructible_v<U(*)[], T(*)[]>)
        {
        }

        template<typename U>
        void operator()(U* pointer) const noexcept
            requires (std::is_constructible_v<U(*)[], T(*)[]>)
        {
            static_assert(sizeof(U) > 0, "incompleted type");
            delete[] pointer;
        }
    };

    /**
     * @brief 参照カウンターのタイプ。
    */
    enum class ThreadPolicy : uint8_t
    {
        NotThreadSafe = 0,
        ThreadSafe
    };

    namespace detail
    {
        // detection idiom
        template<typename T, typename Deleter, class = void>
        struct TDetectUniquePtrType
        {
            using type = T*;
        };

        template<typename T, typename Deleter>
        struct TDetectUniquePtrType<T, Deleter, std::void_t<typename Deleter::pointer>>
        {
            using type = typename Deleter::pointer;
        };

        // detect idiom
        template<typename T, class = void>
        struct IsDeletable : std::false_type {};
        template<typename T>
        struct IsDeletable<T, std::void_t<decltype(delete std::declval<T>())>> : std::true_type {};

        template<typename T, class = void>
        struct IsArrayDeletable : std::false_type {};
        template<typename T>
        struct IsArrayDeletable<T, std::void_t<decltype(delete[] std::declval<T>())>> : std::true_type {};

        // @third party code - BEGIN STL
        // Copyright (c) Microsoft Corporation.
        // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
        template<typename From, typename To>
        struct IsSharedPointerConvertable : std::is_convertible<From*, To*>::type {};
        template<typename From, typename To>
        struct IsSharedPointerConvertable<From, To[]> : std::is_convertible<From(*)[], To(*)[]>::type {};
        template<typename From, typename To, std::size_t Extent>
        struct IsSharedPointerConvertable<From, To[Extent]> : std::is_convertible<From(*)[Extent], To(*)[Extent]>::type {};

        template <class Func, class Arg, class = void>
        struct CanCallFunctionObject : std::false_type {};
        template <class Func, class Arg>
        struct CanCallFunctionObject<Func, Arg, std::void_t<decltype(std::declval<Func>()(std::declval<Arg>()))>> : std::true_type {};
        // @third party code - END STL

        template<ThreadPolicy> struct RefCounterTypeSelector;
        template<>
        struct RefCounterTypeSelector<ThreadPolicy::NotThreadSafe>
        {
            using type = int32_t;
        };

        template<>
        struct RefCounterTypeSelector<ThreadPolicy::ThreadSafe>
        {
            using type = std::atomic<int32_t>;
        };

        /**
         * @brief 参照カウンターの基底クラス。参照カウンターの増減のみ管理する。
        */
        template<ThreadPolicy Policy>
        struct TRefCounterBase
        {
            using counter_type = typename RefCounterTypeSelector<Policy>::type;

            TRefCounterBase() noexcept = default;
            TRefCounterBase(const TRefCounterBase&) = delete;
            TRefCounterBase& operator=(const TRefCounterBase&) = delete;
            virtual ~TRefCounterBase() noexcept = default;

            virtual void destroy() = 0;

            void addReference() noexcept
            {
                if constexpr (Policy == ThreadPolicy::ThreadSafe)
                {
                    _numUses.fetch_add(1, std::memory_order_relaxed);
                }
                else
                {
                    ++_numUses;
                }
            }

            void release()
            {
                if constexpr (Policy == ThreadPolicy::ThreadSafe)
                {
                    if (_numUses.fetch_sub(1, std::memory_order_release) == 1)
                    {
                        std::atomic_thread_fence(std::memory_order_acquire);
                        destroy();
                        delete this;
                    }
                }
                else
                {
                    --_numUses;
                    if (_numUses == 0)
                    {
                        destroy();
                        delete this;
                    }
                }
            }

            [[nodiscard]] int32_t getUseCount() const noexcept
            {
                return _numUses;
            }

        private:
            counter_type _numUses{ 1 };
            counter_type _numWeakUses{ 1 };
        };

        /**
         * @brief オブジェクトの所有権を参照カウンターを持ち、解放処理を行うクラス。
         * @tparam T 実際のオブジェクト。所有権を持っている。
        */
        template<typename T, ThreadPolicy Policy, typename Deleter>
        class TRefCounter final : public TRefCounterBase<Policy>
        {
        public:
            TRefCounter(T* pointer, Deleter&& deleter)
                : _ownedPointer{ pointer }
                , _deleter{ deleter }
            {
            }

            void destroy() override
            {
                _deleter(_ownedPointer);
                _ownedPointer = nullptr;
            }
        private:
            /** インスタンスへのポインタ。このクラスが所有権を持っている。 */
            T* _ownedPointer{ nullptr };

            [[no_unique_address]]
            Deleter _deleter{};
        };
    }


    /**
    * @brief 所有権を持つポインタ。コピーすることはできない。
    */
    template<typename T, typename Deleter = TDefaultDelete<T>>
    class TUniquePtr
    {
    public:
        // deleter_type::pointerがある場合は、pointerはその型になる。
        using element_type = T;
        using deleter_type = Deleter;
        using pointer = typename detail::TDetectUniquePtrType<T, std::remove_reference_t<deleter_type>>::type;

        constexpr TUniquePtr() noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
            : _pointer{ nullptr }
            , _deleter{}
        {
        }

        explicit TUniquePtr(pointer ptr) noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
            : _pointer{ ptr }
            , _deleter{}
        {
        }

        TUniquePtr(pointer ptr, const deleter_type& deleter) noexcept
            requires(std::is_nothrow_copy_constructible_v<deleter_type>)
            : _pointer{ ptr }
            , _deleter{ deleter }
        {
        }

        TUniquePtr(TUniquePtr&& ptr) noexcept
            requires(std::is_nothrow_move_constructible_v<deleter_type>)
            : _pointer{ ptr.release() }
            , _deleter{ std::forward<deleter_type>(ptr.getDeleter()) }
        {
        }

        constexpr TUniquePtr(decltype(nullptr)) noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
            : _pointer{ nullptr }
        {
        }

        template<typename T2, class Deleter2>
        TUniquePtr(TUniquePtr<T2, Deleter2>&& other) noexcept
            requires(
            std::is_convertible_v<typename TUniquePtr<T2, Deleter2>::pointer, pointer>&&
            std::negation_v<std::is_array<T2>>&&
            std::is_reference_v<deleter_type> ? std::is_convertible_v<Deleter2, deleter_type> : std::is_convertible_v<Deleter2, deleter_type>)
            : _pointer{ other.release() }
            , _deleter{ std::forward<deleter_type>(other.getDeleter()) }
        {
        }

        TUniquePtr(const TUniquePtr&) = delete;
        TUniquePtr& operator=(const TUniquePtr&) = delete;

        TUniquePtr& operator=(TUniquePtr&& other) noexcept
            requires(std::is_nothrow_move_constructible_v<deleter_type>)
        {
            reset(other.release());
            _deleter = std::forward<deleter_type>(other._deleter);
            return *this;
        }

        template <class T2, class Deleter2>
        TUniquePtr& operator=(TUniquePtr<T2, Deleter2>&& other) noexcept
            requires(
        std::is_convertible_v<typename TUniquePtr<T2, Deleter2>::pointer, pointer>&&
            std::negation_v<std::is_array<T2>>&&
            std::is_assignable_v<deleter_type&, Deleter2>)
        {
            reset(other.release());
            _deleter = std::forward<Deleter2>(other.getDeleter());
            return *this;
        }

        TUniquePtr& operator=(decltype(nullptr)) noexcept
        {
            reset();
            return *this;
        }

        ~TUniquePtr() noexcept
        {
            if (_pointer)
            {
                _deleter(_pointer);
            }
        }

        [[nodiscard]] std::add_lvalue_reference_t<T> operator*() const noexcept
        {
            return *_pointer;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            ZEN_ASSERT_SLOW(isValid());
            return _pointer;
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return _pointer != nullptr;
        }

        [[nodiscard]] bool isValid() const noexcept
        {
            return _pointer != nullptr;
        }

        pointer release()
        {
            pointer const temp{ _pointer };
            _pointer = nullptr;
            return temp;
        }

        void reset(pointer ptr = pointer()) noexcept
        {
            pointer oldPtr{ _pointer };
            _pointer = ptr;
            if (oldPtr)
            {
                _deleter(oldPtr);
            }
        }

        void swap(TUniquePtr& other) noexcept
        {
            std::swap(_pointer, other._pointer);
            std::swap(_deleter, other._deleter);
        }

        [[nodiscard]] Deleter& getDeleter() noexcept
        {
            return _deleter;
        }

        [[nodiscard]] const Deleter& getDeleter() const noexcept
        {
            return _deleter;
        }

        [[nodiscard]] element_type* get() const noexcept
        {
            return _pointer;
        }

    private:
        element_type* _pointer;
        [[no_unique_address]]
        Deleter _deleter;
    };

    template<typename T, typename Deleter>
    class TUniquePtr<T[], Deleter>
    {
    public:
        using element_type = T;
        using deleter_type = Deleter;
        using pointer = typename detail::TDetectUniquePtrType<T, std::remove_reference_t<deleter_type>>::type;

        constexpr TUniquePtr() noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
            : _pointer{ nullptr }
            , _deleter{}
        {
        }

        template<typename U>
        explicit TUniquePtr(U ptr) noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
            : _pointer{ ptr }
            , _deleter{}
        {
        }

        TUniquePtr(const TUniquePtr&) = delete;
        TUniquePtr& operator=(const TUniquePtr&) = delete;

        [[nodiscard]] pointer operator[](std::size_t index) const noexcept
        {
            return _pointer[index];
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return _pointer != nullptr;
        }

        pointer release() noexcept
        {
            pointer const temp{ _pointer };
            _pointer = nullptr;
            return temp;
        }

        [[nodiscard]] Deleter& getDeleter() noexcept
        {
            return _deleter;
        }

        [[nodiscard]] const Deleter& getDeleter() const noexcept
        {
            return _deleter;
        }

        [[nodiscard]] element_type* get() const noexcept
        {
            return _pointer;
        }

    private:
        element_type* _pointer;

        [[no_unique_address]] Deleter _deleter;
    };

    template<typename T, ThreadPolicy Policy = ThreadPolicy::NotThreadSafe>
    class TSharedPtr
    {
    public:
        using element_type = std::remove_extent_t<T>;

        constexpr TSharedPtr() noexcept
            : _pointer{ nullptr }
            , _refCounter{ nullptr }
        {
        }


        template<typename U>
        explicit TSharedPtr(U* ptr)
            requires(std::is_convertible_v<U*, element_type*>
            && sizeof(U) > 0
            && std::conditional_t<std::is_array_v<T>, detail::IsArrayDeletable<U*>, detail::IsDeletable<U*>>::value
            && detail::IsSharedPointerConvertable<U, T>::value)
            : _pointer{ ptr }
            , _refCounter{ nullptr }
        {
            if constexpr (std::is_array_v<U>)
            {
                allocateInternal(_pointer, TDefaultDelete<element_type[]>{});
            }
            else
            {
                allocateInternal(_pointer, TDefaultDelete<element_type>{});
            }
        }

        template<typename U, typename Deleter>
        TSharedPtr(U* ptr, Deleter deleter)
            requires(std::is_move_constructible_v<Deleter>
        && detail::IsSharedPointerConvertable<U, T>::value
            && detail::CanCallFunctionObject<Deleter&, U*&>::value)
            : _pointer{ ptr }
            , _refCounter{ nullptr }
        {
            allocateInternal(_pointer, deleter);
        }

        template<typename Deleter>
            requires(std::is_move_constructible_v<Deleter>
            && detail::CanCallFunctionObject<Deleter&, std::nullptr_t&>::value)
            TSharedPtr(std::nullptr_t pointer, Deleter deleter)
        {
            //@TODO
        }

        template<typename Deleter, typename Allocator>
            requires(std::is_move_constructible_v<Deleter>
        && detail::CanCallFunctionObject<Deleter&, std::nullptr_t&>::value)
            TSharedPtr(std::nullptr_t pointer, Deleter deleter, Allocator allocator)
        {
            //@TODO
        }

        TSharedPtr(const TSharedPtr& other) noexcept
            : _pointer{ other._pointer }
            , _refCounter{ other._refCounter }
        {
            if (_refCounter)
            {
                _refCounter->addReference();
            }
        }

        TSharedPtr(TSharedPtr<T>&& other) noexcept
            : _pointer{ other._pointer }
            , _refCounter{ other._refCounter }
        {
            other._pointer = nullptr;
            other._refCounter = nullptr;
        }

        template<typename U>
        TSharedPtr(TSharedPtr<U>&& other) noexcept
            requires(std::is_convertible_v<U*, element_type*>)
        : _pointer{ other._pointer }
            , _refCounter{ other._refCounter }
        {
            other._pointer = nullptr;
            other._refCount = nullptr;
        }

        constexpr TSharedPtr(std::nullptr_t) noexcept
            : _pointer{ nullptr }
            , _refCounter{ nullptr }
        {
        }

        ~TSharedPtr() noexcept
        {
            if (_refCounter)
            {
                _refCounter->release();
            }
        }

        [[nodiscard]] std::add_lvalue_reference_t<T> operator*() const noexcept
        {
            return *_pointer;
        }

        [[nodiscard]] element_type* operator->() const noexcept
        {
            ZEN_ASSERT_SLOW(isValid());
            return _pointer;
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return _pointer != nullptr;
        }

        [[nodiscard]] bool isValid() const noexcept
        {
            return _pointer != nullptr;
        }

        [[nodiscard]] element_type* get() const noexcept
        {
            return _pointer;
        }

        [[nodiscard]] int32_t getUseCount() const noexcept
        {
            return _refCounter ? _refCounter->getUseCount() : 0;
        }

    private:
        template<typename U, typename Deleter>
        void allocateInternal(U* pointer, Deleter deleter)
        {
            std::unique_ptr<U> scopedTempOwner(pointer);
            _refCounter = new detail::TRefCounter<element_type, Policy, Deleter>(pointer, std::move(deleter));
            scopedTempOwner.release();
        }

        template<typename U, typename Deleter, typename Allocator>
        void allocateInternal()
        {
            // @TODO
        }

        element_type* _pointer;
        detail::TRefCounterBase<Policy>* _refCounter; ///< 参照カウンターと所有しているポインタ
    };

    template<typename T, typename... Args>
    [[nodiscard]] TUniquePtr<T> makeUnique(Args&&... args)
        requires(!std::is_array_v<T>)
    {
        return TUniquePtr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    [[nodiscard]] TUniquePtr<T> makeUnique(size_t n)
        requires(std::is_array_v<T>&& std::extent_v<T>)
    {
        return TUniquePtr<T>(new std::remove_extent_t<T>[n]());
    }

    template <class T, class... Args>
    void makeUnique(Args&&...)
        requires(std::is_bounded_array_v<T>) = delete;

    template<typename T1, typename Deleter1, typename T2, typename Deleter2>
    [[nodiscard]] bool operator==(const TUniquePtr<T1, Deleter1>& lhs, const TUniquePtr<T2, Deleter2>& rhs)
    {
        return lhs.get() == rhs.get();
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator==(const TUniquePtr<T, Deleter>& lhs, std::nullptr_t)
    {
        return !lhs;
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator==(std::nullptr_t, const TUniquePtr<T, Deleter>& rhs)
    {
        return !rhs;
    }

    template<typename T1, typename Deleter1, typename T2, typename Deleter2>
    [[nodiscard]] bool operator!=(const TUniquePtr<T1, Deleter1>& lhs, const TUniquePtr<T2, Deleter2>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator!=(const TUniquePtr<T, Deleter>& lhs, std::nullptr_t)
    {
        return static_cast<bool>(lhs);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator!=(std::nullptr_t, const TUniquePtr<T, Deleter>& rhs)
    {
        return static_cast<bool>(rhs);
    }

    template<typename T1, typename Deleter1, typename T2, typename Deleter2>
    [[nodiscard]] bool operator<(const TUniquePtr<T1, Deleter1>& lhs, const TUniquePtr<T2, Deleter2>& rhs)
    {
        using CT = std::common_type_t<typename TUniquePtr<T1, Deleter1>::pointer, typename TUniquePtr<T2, Deleter2>::pointer>;
        return std::less<CT>(lhs.get(), rhs.get());
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator<(const TUniquePtr<T, Deleter>& lhs, std::nullptr_t)
    {
        return std::less<typename TUniquePtr<T, Deleter>::pointer>()(lhs.get(), nullptr);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator<(std::nullptr_t, const TUniquePtr<T, Deleter>& rhs)
    {
        return std::less<typename TUniquePtr<T, Deleter>::pointer>()(nullptr, rhs.get());
    }

    template<typename T1, typename Deleter1, typename T2, typename Deleter2>
    [[nodiscard]] bool operator<=(const TUniquePtr<T1, Deleter1>& lhs, const TUniquePtr<T2, Deleter2>& rhs)
    {
        return !(rhs < lhs);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator<=(const TUniquePtr<T, Deleter>& lhs, std::nullptr_t)
    {
        return !(nullptr < lhs);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator<=(std::nullptr_t, const TUniquePtr<T, Deleter>& rhs)
    {
        return !(rhs < nullptr);
    }

    template<typename T1, typename Deleter1, typename T2, typename Deleter2>
    [[nodiscard]] bool operator>(const TUniquePtr<T1, Deleter1>& lhs, const TUniquePtr<T2, Deleter2>& rhs)
    {
        return rhs < lhs;
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator>(const TUniquePtr<T, Deleter>& lhs, std::nullptr_t)
    {
        return nullptr < lhs;
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator>(std::nullptr_t, const TUniquePtr<T, Deleter>& rhs)
    {
        return rhs < nullptr;
    }

    template<typename T1, typename Deleter1, typename T2, typename Deleter2>
    [[nodiscard]] bool operator>=(const TUniquePtr<T1, Deleter1>& lhs, const TUniquePtr<T2, Deleter2>& rhs)
    {
        return !(lhs < rhs);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator>=(const TUniquePtr<T, Deleter>& lhs, std::nullptr_t)
    {
        return !(lhs < nullptr);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator>=(std::nullptr_t, const TUniquePtr<T, Deleter>& rhs)
    {
        return !(nullptr < rhs);
    }
}
