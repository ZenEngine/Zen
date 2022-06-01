#pragma once
#include <string>

namespace zen
{
    class String final
    {
    public:
        using string_type = std::u16string;

        using traits_type = typename string_type::traits_type;
        using allocator_type = typename string_type::allocator_type;

        using value_type = typename string_type::value_type;
        using size_type = typename string_type::size_type;
        using difference_type = typename string_type::difference_type;
        using pointer = typename string_type::pointer;
        using const_pointer = typename string_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;

        using iterator = typename string_type::iterator;
        using const_iterator = typename string_type::const_iterator;

        using reverse_iterator = typename string_type::reverse_iterator;
        using const_reverse_iterator = typename string_type::const_reverse_iterator;

        static constexpr size_type npos{ static_cast<size_type>(-1) };


    private:
        string_type _data;
    };
}