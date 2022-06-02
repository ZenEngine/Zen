#pragma once
#include <Core/Misc/Assert.hpp>
#include <cstddef>
#include <type_traits>
#include <iterator>

namespace zen
{
    inline constexpr std::size_t dynamicExtent{ static_cast<std::size_t>(-1) };

    namespace details
    {

    }

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

        [[nodiscard]]
        constexpr bool isEmpty() const noexcept { return getSize() == 0; }

        [[nodiscard]]
        constexpr reference front() const
        {
            ZEN_EXPECTS(!isEmpty());
            return *getData();
        }

        [[nodiscard]]
        constexpr reference back() const
        {
            ZEN_EXPECTS(!isEmpty());
            return  *(getData() + (getSize() - 1));
        }

        [[nodiscard]]
        constexpr pointer getData() const noexcept { return _ptr; }

        [[nodiscard]]
        constexpr iterator begin() const noexcept
        {
            return _ptr;
        }

        friend constexpr iterator begin(TSpan s) noexcept
        {
            return s.begin();
        }

        [[nodiscard]]
        constexpr iterator end() const noexcept
        {
            return _ptr + _size;
        }

        friend constexpr iterator end(TSpan s) noexcept
        {
            return s.end();
        }

        [[nodiscard]]
        constexpr reverse_iterator rbegin() const noexcept
        {
            return reverse_iterator(end());
        }

        [[nodiscard]]
        constexpr reverse_iterator rend() const noexcept
        {
            return reverse_iterator(begin());
        }

    private:
        pointer _ptr;
        size_type _size;
    };
}