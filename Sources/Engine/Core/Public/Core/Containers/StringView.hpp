#pragma once
#include <Core/Misc/Assert.hpp>
#include <iterator>
#include <cstddef>
#include <numeric>

namespace zen
{
    class StringView
    {
    public:
        using CharT = char16_t;
        // stringヘッダーにあるので、依存させたくない。
        //using trait_type = std::char_traits<char16_t>;
        using value_type = CharT;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_iterator = const_pointer;
        using iterator = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;
        using const_reference = const value_type&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        constexpr StringView() noexcept
            : _pointer{ nullptr }
            , _size{ 0 }
        {
        }

        constexpr StringView(const StringView&) noexcept = default;
        constexpr StringView(const CharT* str)
            : _pointer{ str }
            , _size{ /* trait_type::length(str) */ }
        {
            // @TODO
        }

        constexpr StringView(const CharT* str, size_type length)
            : _pointer{ str }
            , _size{ length }
        {
        }

        ~StringView() noexcept = default;

        [[nodiscard]] constexpr size_type getSize() const noexcept
        {
            return _size;
        }

        [[nodiscard]] constexpr size_type getMaxSize() const noexcept
        {
            return (std::numeric_limits<size_type>::max)();
        }

        [[nodiscard]] constexpr const_pointer getData() const noexcept
        {
            return _pointer;
        }

        [[nodiscard]] constexpr bool isEmpty() const noexcept
        {
            return _size == 0;
        }

        [[nodiscard]] constexpr const_reference operator[](size_type pos) const
        {
            ZEN_EXPECTS_MSG(pos < _size, u"Index out of range.");
            return _pointer[pos];
        }

    protected:
        const_pointer _pointer;
        size_type _size;
    };
}