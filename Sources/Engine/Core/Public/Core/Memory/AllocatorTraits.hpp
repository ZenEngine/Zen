#pragma once
#include "PointerTraits.hpp"
#include <type_traits>
#include <limits>
#include <utility>

namespace zen
{
    // @third party code - BEGIN STL
    // Copyright (c) Microsoft Corporation.
    // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
    namespace detail::allocator_traits
    {
        template<typename Allocator, class = void>
        struct TDetectValueType
        {
            using type = typename Allocator::value_type*;
        };

        template<typename Allocator>
        struct TDetectValueType<Allocator, std::void_t<typename Allocator::pointer>>
        {
            using type = typename Allocator::pointer;
        };

        template<typename Allocator, class = void>
        struct TDetectConstPointer
        {
            using pointer = typename detail::allocator_traits::TDetectValueType<Allocator>::type;
            using value_type = typename Allocator::value_type;
            using type = typename PointerTraits<pointer>::template rebind<const value_type>;
        };

        template<typename Allocator>
        struct TDetectConstPointer<Allocator, std::void_t<typename Allocator::const_pointer>>
        {
            using type = typename Allocator::const_pointer;
        };

        template<typename Allocator, class = void>
        struct TDetectVoidPointer
        {
            using pointer = typename detail::allocator_traits::TDetectValueType<Allocator>::type;
            using type = typename PointerTraits<pointer>::template rebind<void>;
        };

        template<typename Allocator>
        struct TDetectVoidPointer<Allocator, std::void_t<typename Allocator::void_pointer>>
        {
            using type = typename Allocator::void_pointer;
        };

        template<typename Allocator, class = void>
        struct TDetectDifferenceType
        {
            using pointer = typename detail::allocator_traits::TDetectValueType<Allocator>::type;
            using type = typename PointerTraits<pointer>::template difference_type;
        };

        template<typename Allocator>
        struct TDetectDifferenceType<Allocator, std::void_t<typename Allocator::difference_type>>
        {
            using type = typename Allocator::difference_type;
        };

        template<typename Allocator, class = void>
        struct TDetectSizeType
        {
            using difference_type = typename TDetectDifferenceType<Allocator>::type;
            using type = typename std::make_unsigned<difference_type>::template difference_type;
        };

        template<typename Allocator>
        struct TDetectSizeType<Allocator, std::void_t<typename Allocator::size_type>>
        {
            using type = typename Allocator::size_type;
        };

        template<typename Allocator, class = void>
        struct TDetectPropagateOnContainerCopyAssignment
        {
            using type = std::false_type;
        };

        template<typename Allocator>
        struct TDetectPropagateOnContainerCopyAssignment<Allocator, std::void_t<typename Allocator::propagate_on_container_copy_assignment>>
        {
            using type = Allocator::propagate_on_container_copy_assignment;
        };

        template<typename Allocator, class = void>
        struct TDetectPropagateOnContainerMoveAssignment
        {
            using type = std::false_type;
        };

        template<typename Allocator>
        struct TDetectPropagateOnContainerMoveAssignment<Allocator, std::void_t<typename Allocator::propagate_on_container_move_assignment>>
        {
            using type = Allocator::propagate_on_container_move_assignment;
        };

        template<typename Allocator, class = void>
        struct TDetectPropagateOnContainerSwap
        {
            using type = std::false_type;
        };

        template<typename Allocator>
        struct TDetectPropagateOnContainerSwap<Allocator, std::void_t<typename Allocator::propagate_on_container_swap>>
        {
            using type = Allocator::propagate_on_container_swap;
        };

        template<typename Allocator, class = void>
        struct TDetectIsAlwaysEqual
        {
            using type = std::bool_constant<std::is_empty_v<Allocator>>;
        };

        template<typename Allocator>
        struct TDetectIsAlwaysEqual<Allocator, std::void_t<typename Allocator::is_always_equal>>
        {
            using type = typename Allocator::is_always_equal;
        };

        template <class Newfirst, class T>
        struct ReplaceFirstParameter;

        template <class Newfirst, template <class, class...> class T, class First, class... Rest>
        struct ReplaceFirstParameter<Newfirst, T<First, Rest...>> { // given _Ty<_First, _Rest...>, replace _First
            using type = T<Newfirst, Rest...>;
        };

        template <typename Allocator, typename U, class = void>
        struct TDetectRebindType
        {
            using type = typename ReplaceFirstParameter<U, Allocator>::type;
        };


        template <typename Allocator, typename U>
        struct TDetectRebindType<Allocator, U, std::void_t<typename Allocator::template rebind<U>::other>>
        {
            using type = typename Allocator::template rebind<U>::other;
        };
    }

    template<typename Allocator>
    struct AllocatorTraits
    {
        using allocator_type = Allocator;
        using value_type = typename Allocator::value_type;
        using pointer = typename detail::allocator_traits::TDetectValueType<allocator_type>::type;
        using const_pointer = typename detail::allocator_traits::TDetectConstPointer<allocator_type>::type;
        using void_pointer = typename detail::allocator_traits::TDetectVoidPointer<allocator_type>::type;
        using difference_type = typename detail::allocator_traits::TDetectDifferenceType<allocator_type>::type;
        using size_type = typename detail::allocator_traits::TDetectSizeType<allocator_type>::type;
        using propagate_on_container_copy_assignment = typename detail::allocator_traits::TDetectPropagateOnContainerCopyAssignment<allocator_type>::type;
        using propagate_on_container_move_assignment = typename detail::allocator_traits::TDetectPropagateOnContainerMoveAssignment<allocator_type>::type;
        using propagate_on_container_swap = typename detail::allocator_traits::TDetectPropagateOnContainerSwap<allocator_type>::type;
        using is_always_equal = typename detail::allocator_traits::TDetectIsAlwaysEqual<allocator_type>::type;

        template <typename U>
        using rebind_alloc = typename detail::allocator_traits::TDetectRebindType<allocator_type, U>::type;

        template <typename U>
        using rebind_traits = AllocatorTraits<rebind_alloc<U>>;

        [[nodiscard]] static constexpr pointer allocate(Allocator& allocator, size_type n)
        {
            return allocator.allocate(n);
        }

        //[[nodiscard]] static constexpr pointer allocate(Allocator& allocator, size_type n,
        //    const_void_pointer hint)
        //{

        //}

        static constexpr void deallocate(Allocator& allocator, pointer p, size_type n)
        {
            allocator.deallocate(p, n);
        }

        //static constexpr size_type max_size(const Allocator& allocator) noexcept
        //{
        //    return (std::numeric_limits<size_type>::max)() / sizeof(value_type);
        //}

        //template <class T, class... Args>
        //static constexpr void construct(Allocator& allocator, T* p, Args&&... args)
        //{
        //    //construct_at(p, std::forward<Args>(args)...)p
        //}

        //template <class T>
        //static constexpr void destroy(Allocator& allocator, T* p)
        //{

        //}

        //static constexpr Allocator select_on_container_copy_construction(const Allocator& a)
        //{

        //}
    };
}