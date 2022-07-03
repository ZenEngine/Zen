#pragma once
#include <type_traits>
#include <memory> // @TODO: Delete pointer_traisを移植し、to_addressのSTL依存を外す。

namespace zen
{
    /**
     * @brief std::addressof
    */
    template<typename T>
    [[nodiscard]] constexpr T* addressof(T& value) noexcept
    {
        return __builtin_addressof(value);
    }

    template <class T>
    const T* addressof(const T&& value) = delete;

    template<typename T>
    constexpr T* toAddress(T* ptr) noexcept
    {
        static_assert(!std::is_function_v<T>);
        return ptr;
    }

    template<typename T>
    constexpr auto toAddress(const T& ptr) noexcept
    {
        // @TODO: ここを独自に変えると、STL(memory)から切り離せる。
        return std::to_address(ptr);
    }
}