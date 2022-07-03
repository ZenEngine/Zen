#pragma once
#include "MemoryUtils.hpp"
#include <cstddef>
#include <type_traits>

namespace zen
{
    // @third party code - BEGIN STL
    // Copyright (c) Microsoft Corporation.
    // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
    namespace detail::pointer_traits
    {
        template <class T>
        struct GetFirstParameter;

        template <template <class, class...> class T, class First, class... Rest>
        struct GetFirstParameter<T<First, Rest...>>
        {
            using type = First;
        };

        template <class Newfirst, class T>
        struct ReplaceFirstParameter;

        template <class Newfirst, template <class, class...> class T, class First, class... Rest>
        struct ReplaceFirstParameter<Newfirst, T<First, Rest...>>
        {
            using type = T<Newfirst, Rest...>;
        };

        template <class T, class = void>
        struct GetElementType
        {
            using type = typename GetFirstParameter<T>::type;
        };

        template <class Ty>
        struct GetElementType<Ty, std::void_t<typename Ty::element_type>>
        {
            using type = typename Ty::element_type;
        };

        template <class T, class = void>
        struct GetPtrDifferenceType
        {
            using type = ptrdiff_t;
        };

        template <class T>
        struct GetPtrDifferenceType<T, std::void_t<typename T::difference_type>>
        {
            using type = typename T::difference_type;
        };

        template <class T, class Other, class = void>
        struct GetRebindAlias {
            using type = typename ReplaceFirstParameter<Other, T>::type;
        };

        template <class T, class Other>
        struct GetRebindAlias<T, Other, std::void_t<typename T::template rebind<Other>>>
        {
            using type = typename T::template rebind<Other>;
        };
    }

    template<typename Ptr>
    struct PointerTraits
    {
        using pointer = Ptr;
        using element_type = typename detail::pointer_traits::GetElementType<Ptr>::type;
        using difference_type = typename detail::pointer_traits::GetPtrDifferenceType<Ptr>::type;

        template <class Other>
        using rebind = typename detail::pointer_traits::GetRebindAlias<Ptr, Other>::type;

        using ReferenceType = std::conditional_t<std::is_void_v<element_type>, char, element_type>&;

        [[nodiscard]]
        static pointer pointerTo(ReferenceType _Val) noexcept(noexcept(Ptr::pointerTo(_Val)))
        {
            return Ptr::pointerTo(_Val);
        }
    };

    template<typename T>
    struct PointerTraits<T*>
    {
        using pointer = T*;
        using element_type = T;
        using difference_type = std::ptrdiff_t;

        template<typename U>
        using rebind = U*;

        using ReferenceType = std::conditional_t<std::is_void_v<T>, char, T>&;

        [[nodiscard]]
        static constexpr pointer pointerTo(ReferenceType value) noexcept
        {
            return addressof(value);
        }
    };
    // @third party code - END STL
}