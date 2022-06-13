#pragma once
#include <cstddef>

#if defined(_MSC_VER)
#include <cstring> // for std::memcpy
#endif

namespace zen
{
    namespace detail
    {
        // char_traits相当。
        namespace StringHelper
        {
            constexpr bool eq(const char16_t c1, const char16_t c2) noexcept
            {
                return c1 == c2;
            }

            constexpr bool lt(const char16_t c1, const char16_t c2) noexcept
            {
                return c1 < c2;
            }

            constexpr std::size_t length(const char16_t* str) noexcept
            {
                size_t result{ 0 };
                for (; *str != char16_t{}; ++str)
                {
                    ++result;
                }
                return result;
            }


            constexpr int compare(const char16_t* s1, const char16_t* s2, std::size_t n) noexcept
            {
                for (; n; --n, ++s1, ++s2)
                {
                    if (lt(*s1, *s2))
                    {
                        return -1;
                    }
                    if (lt(*s2, *s1))
                    {
                        return 1;
                    }
                }
                return 0;
            }

            constexpr char16_t* copy(char16_t* s1, const char16_t* s2, std::size_t n)
            {
#if defined(_MSC_VER)
                if (std::is_constant_evaluated())
                {
                    for (size_t index{ 0 }; index < n; ++index)
                    {
                        s1[index] = s2[index];
                    }
                    return s1;
                }
                else
                {
                    std::memcpy(s1, s2, sizeof(char16_t) * n);
                    return s1;
                }
#else
                __builtin_memcpy(s1, s2, sizeof(char16_t) * n);
                return s1;
#endif
            }

            constexpr const char16_t* find(const char16_t* p, size_t size, const char16_t& c)
            {
                for (size_t index{ 0 }; index < size; ++index, ++p)
                {
                    if (*p == c)
                    {
                        return p;
                    }
                }

                return nullptr;
            }

            // @third party code - BEGIN STL
            // Copyright (c) Microsoft Corporation.
            // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
            constexpr size_t find(
                const char16_t* s1, size_t size1,
                size_t pos,
                const char16_t* s2, size_t size2) noexcept
            {
                if (size2 > size1 || pos > size1 - size2)
                {
                    return static_cast<size_t>(-1);
                }

                if (size2 == 0)
                {
                    return pos;
                }

                const char16_t* const possibleMatchesEnd{ s1 + (size1 - size2) + 1 };
                for (const char16_t* target{ s1 + pos };; ++target)
                {
                    target = find(target, static_cast<size_t>(possibleMatchesEnd - target), *s2);
                    if (!target)
                    {
                        return static_cast<size_t>(-1);
                    }

                    if (compare(target, s2, size2) == 0)
                    {
                        return static_cast<size_t>(target - s1);
                    }
                }

            }

            constexpr size_t rfind(const char16_t* s1, const size_t size1,
                const size_t pos, const char16_t* s2,
                const size_t size2) noexcept {
                if (size2 == 0) {
                    // min
                    return size1 < pos ? size1 : pos; // empty string always matches
                }

                if (size2 <= size1) { // room for match, look for it
                    for (auto target = s1 + (size1 - size2 < pos ? size1 - size2 : pos);; --target) {
                        if (eq(*target, *s2) && compare(target, s2, size2) == 0) {
                            return static_cast<size_t>(target - s1); // found a match
                        }

                        if (target == s1) {
                            break; // at beginning, no more chance for match
                        }
                    }
                }

                return static_cast<size_t>(-1); // no match
            }

            constexpr size_t findFirstOf(
                const char16_t* s1, const size_t size1,
                const size_t pos,
                const char16_t* s2, const size_t size2) noexcept
            {
                const char16_t* const end = s1 + size1;
                for (const char16_t* target = s1 + pos; target < end; ++target) {
                    if (find(s2, size2, *target)) {
                        return static_cast<size_t>(target - s1); // found a match
                    }
                }
                return static_cast<size_t>(-1); // no match
            }


            constexpr size_t findLastOf(
                const char16_t* s1, const size_t size1,
                const size_t pos,
                const char16_t* s2, const size_t size2) noexcept
            {
                for (auto target = s1 + (size1 - 1 < pos ? size1 - 1 : pos);; --target) {
                    if (find(s2, size2, *target)) {
                        return static_cast<size_t>(target - s1); // found a match
                    }

                    if (target == s1) {
                        break; // at beginning, no more chance for match
                    }
                }
                return static_cast<size_t>(-1); // no match
            }

            constexpr size_t findFirstNotOf(
                const char16_t* s1, const size_t size1,
                const size_t pos,
                const char16_t* s2, const size_t size2) noexcept
            {
                if (pos < size1)
                {
                    for (const char16_t* target{ s1 + pos }; target < s2 + size2; ++target)
                    {
                        if (!find(s1, size1, *target))
                        {
                            return static_cast<size_t>(target - s1);
                        }
                    }
                }

                return static_cast<size_t>(-1);
            }

            constexpr size_t findLastNotOf(
                const char16_t* s1, const size_t size1,
                const size_t pos,
                const char16_t* s2, const size_t size2) noexcept
            {
                if (pos < size1)
                {
                    for (const char16_t* target{ s1 +  (size1 - 1< pos ? size1 - 1 : pos)};; --target)
                    {
                        if (!find(s2, size2, *target))
                        {
                            return static_cast<size_t>(target - s1);
                        }

                        if (s1 == target)
                        {
                            break;
                        }
                    }
                }

                return static_cast<size_t>(-1);
            }

            // @third party code - END STL
        }
    }
}