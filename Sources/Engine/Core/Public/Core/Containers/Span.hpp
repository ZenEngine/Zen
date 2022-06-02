#pragma once
#include <Core/Misc/Assert.hpp>
#include <cstddef>
#include <type_traits>
#include <iterator>

namespace zen
{
    inline constexpr std::size_t dynamicExtent{ static_cast<std::size_t>(-1) };

    template<typename ElementType, std::size_t Extent = dynamicExtent>
    class TSpan
    {
    public:
        using element_type = ElementType;
        using value_type = std::remove_cv_t<ElementType>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = element_type*;
        using const_pointer = const element_type*;
        using reference = element_type&;
        using const_reference = const element_type&;

        using iterator = pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;

        static constexpr size_type extent = Extent;

        constexpr TSpan() noexcept requires(extent == dynamicExtent || extent == 0)
            : _ptr{ nullptr }
            , _size{ 0 }
        {
        }

        constexpr TSpan(const TSpan& other) noexcept = default;

        TSpan& operator=(const TSpan&) = default;
        TSpan& operator = (TSpan&&) = default;

        ~TSpan() = default;


        [[nodiscard]]
        constexpr size_type getSize() const noexcept { return _size; }

        [[nodiscard]]
        constexpr size_type getSizeBytes() const noexcept { return _size * sizeof(element_type); }

    private:
        pointer _ptr;
        size_type _size;
    };
}