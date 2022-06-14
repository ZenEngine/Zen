#pragma once
#include <type_traits>
#include <atomic>
#include <utility>

namespace zen
{
    enum class ThreadPolicy : uint8_t
    {
        NotThreadSafe = 0,
        ThreadSafe
    };


    namespace detail
    {
        // detect idiom
        template<typename T, class = void>
        struct IsDeletable : std::false_type{};
        template<typename T>
        struct IsDeletable<T, std::void_t<decltype(delete std::declval<T>())>> : std::true_type{};

        template<typename T, class = void>
        struct IsArrayDeletable : std::false_type{};
        template<typename T>
        struct IsArrayDeletable<T, std::void_t<decltype(delete[] std::declval<T>())>> : std::true_type {};

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

        template<ThreadPolicy Policy>
        struct TRefCounter
        {
            using counter_type = typename RefCounterTypeSelector<Policy>::type;

            TRefCounter() noexcept = default;
            TRefCounter(const TRefCounter&) = delete;
            TRefCounter& operator=(const TRefCounter&) = delete;

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
                        delete this;
                    }
                }
                else
                {
                    --_numUses;
                    if (_numUses == 0)
                    {
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
        };
    }

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
            && std::conditional_t<std::is_array_v<T>, detail::IsArrayDeletable<U*>, detail::IsDeletable<U*>>::value)
            : _pointer{ ptr }
        {
            // @TODO
            if constexpr(std::is_array_v<U>)
            {

            }
            else
            {
                _refCounter = new detail::TRefCounter<Policy>();
            }
        }

        template<typename U, typename Deleter>
        TSharedPtr(U* ptr, Deleter delter)
            requires(
            std::is_convertible_v<U*, element_type*>&&
            std::is_nothrow_copy_constructible_v<Deleter>&&
            std::is_nothrow_destructible_v<Deleter>
            /* @todo */)
        {
            // @todo
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
            : TSharedPtr{}
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
        element_type* _pointer;
        detail::TRefCounter<Policy>* _refCounter;
    };
}
