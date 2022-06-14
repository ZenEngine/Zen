#pragma once
#include <Core/Misc/Assert.hpp>
#include <type_traits>
#include <utility>
#include <cstddef>
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
        using CT = std::common_type_t<TUniquePtr<T1, Deleter1>::pointer, TUniquePtr<T2, Deleter2>::pointer>;
        return std::less<CT>(lhs.get(), rhs.get());
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator<(const TUniquePtr<T, Deleter>& lhs, std::nullptr_t)
    {
        return std::less<TUniquePtr<T, Deleter>::pointer>()(lhs.get(), nullptr);
    }

    template<typename T, typename Deleter>
    [[nodiscard]] bool operator<(std::nullptr_t, const TUniquePtr<T, Deleter>& rhs)
    {
        return std::less<TUniquePtr<T, Deleter>::pointer>()(nullptr, rhs.get());
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
