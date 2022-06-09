#pragma once
#include <type_traits>
#include <atomic>

namespace zen
{
    enum class ThreadPolicy : uint8_t
    {
        NotThreadSafe = 0,
        ThreadSafe
    };


    namespace detail
    {
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
            using counter_type = RefCounterTypeSelector<Policy>::type;

            TRefCounter(const TRefCounter&) = delete;
            TRefCounter& operator=(const TRefCounter&) = delete;

            void addReference() noexcept
            {

            }

            void release()
            {

            }

            [[nodiscard]] int32_t getUseCount() const noexcept
            {
                return _numUses;
            }

        private:
            counter_type _numUses{ 1 };
        };
    }

    template<typename T, ThreadPolicy Policy>
    class TSharedPtr
    {
    public:
        using element_type = std::remove_extent_t<T>;

        constexpr TSharedPtr() noexcept
            : _ptr{ nullptr }
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
            return *_ptr;
        }

        [[nodiscard]] element_type* operator->() const noexcept
        {
            ZEN_ASSERT_SLOW(isValid());
            return _ptr;
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return _ptr != nullptr;
        }

        [[nodiscard]] bool isValid() const noexcept
        {
            return _ptr != nullptr;
        }

        [[nodiscard]] element_type* get() const noexcept
        {
            return _ptr;
        }

        [[nodiscard]] int32_t getUseCount() const noexcept
        {
            return _refCounter ? _refCounter->getUseCount() : 0;
        }

    private:
        element_type* _ptr;
        detail::TRefCounter<Policy>* _refCounter;
    };
}