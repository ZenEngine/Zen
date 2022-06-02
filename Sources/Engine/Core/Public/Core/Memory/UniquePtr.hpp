#pragma once
#include <Core/Misc/Assert.hpp>
#include <type_traits>
#include <utility>

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

        void operator()(T* ptr) const noexcept
        {
            static_assert(sizeof(T) > 0, "Must be not incompleted type.");
            delete ptr;
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
        void operator()(U* ptr) const noexcept
            requires (std::is_constructible_v<U(*)[], T(*)[]>)
        {
            static_assert(sizeof(T) > 0, "Must be not incompleted type.");
            delete[] ptr;
        }
    };

    template<typename T, typename Deleter>
    class TUniquePtr
    {
    public:
        using element_type = T;
        using deleter_type = D;
        using pointer = typename detail::TDetectUniquePtrType<T, std::remove_reference_t<deleter_type>>::type;

        constexpr TUniquePtr() noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
        : _ptr{ nullptr }
            , _deleter{}
        {
        }

        explicit TUniquePtr(pointer ptr) noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
        : _ptr{ ptr }
            , _deleter{}
        {
        }

        TUniquePtr(pointer ptr, const deleter_type& deleter) noexcept
            requires(std::is_nothrow_copy_constructible_v<deleter_type>)
        : _ptr{ ptr }
            , _deleter{ deleter }
        {
        }

        TUniquePtr(TUniquePtr&& ptr) noexcept
            requires(std::is_nothrow_move_constructible_v<deleter_type>)
        : _ptr{ ptr.release() }
            , _deleter{ std::forward<deleter_type>(ptr.getDeleter()) }
        {
        }

        constexpr TUniquePtr(decltype(nullptr)) noexcept
            requires(std::is_nothrow_default_constructible_v<deleter_type>&& std::negation_v<std::is_pointer<deleter_type>>)
        : _ptr{ nullptr }
        {
        }

        template<typename T2, class Deleter2>
        TUniquePtr(TUniquePtr<T2, Deleter2>&& other) noexcept
            requires(
        std::is_convertible_v<typename TUniquePtr<T2, Deleter2>::pointer, pointer>&&
            std::negation_v<std::is_array<T2>>&&
            std::is_reference_v<deleter_type> ? std::is_convertible_v<Deleter2, deleter_type> : std::is_convertible_v<Deleter2, deleter_type>)
            : _ptr{ other.release() }
            , _deleter{ std::forward<deleter_type>(other.getDeleter()) }
        {
        }

        TUniquePtr(const TUniquePtr&) = delete;
        TUniquePtr& operator=(const TUniquePtr&) = delete;

        TUniquePtr& operator=(decltype(nullptr)) noexcept
        {
            reset();
            return *this;
        }

        ~TUniquePtr() noexcept
        {
            if (_ptr)
            {
                _deleter(_ptr);
            }
        }

        [[nodiscard]] std::add_lvalue_reference_t<T> operator*() const noexcept
        {
            return *_ptr;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            ZEN_EXPECTS(isValid());
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

        pointer release()
        {
            pointer const temp{ _ptr };
            _ptr = nullptr;
            return temp;
        }

        void reset(pointer ptr = pointer()) noexcept
        {
            pointer oldPtr{ _ptr };
            _ptr = ptr;
            if (oldPtr)
            {
                _deleter(oldPtr);
            }
        }

        void swap(TUniquePtr& other) noexcept
        {
            std::swap(_ptr, other._ptr);
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
            return _ptr;
        }
    private:
        element_type* _ptr;
        [[no_unique_address]]
        Deleter _deleter;
    };
}