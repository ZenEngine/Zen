#pragma once
#include <iterator>
#include <cstddef>

namespace zen
{
    class StringView
    {
    public:
        using CharT = char16_t;
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
    };
}