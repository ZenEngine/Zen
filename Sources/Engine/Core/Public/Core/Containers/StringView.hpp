#pragma once
#include <Core/Containers/Detail/StringHelper.hpp>
#include <Core/Memory/MemoryUtils.hpp>
#include <Core/Misc/Assert.hpp>
#include <iterator>
#include <cstddef>
#include <numeric>

namespace zen
{
    /**
     * @brief u16string_viewライクな文字列への参照クラス。
    */
    class StringView
    {
    public:
        using char_type = char16_t;

        // ヘッダーでC++標準のstringをincludeしたくないので、traits_typeは非サポート。
        // using trait_type = Not Supported.

        using value_type = char_type;
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

        static constexpr size_type npos{ static_cast<size_type>(-1) };

        constexpr StringView() noexcept
            : _pointer{ nullptr }
            , _size{ 0 }
        {
        }

        constexpr StringView(const StringView&) noexcept = default;
        constexpr StringView(const char_type* str)
            : _pointer{ str }
            , _size{ detail::StringHelper::length(str) }
        {
        }

        constexpr StringView(decltype(nullptr)) = delete;

        constexpr StringView(const char_type* str, size_type length)
            : _pointer{ str }
            , _size{ length }
        {
        }

        template<std::contiguous_iterator Iter, std::sized_sentinel_for<Iter> End>
        requires(std::is_same_v<std::iter_value_t<Iter>, char_type> && !std::is_convertible_v<End, size_type>)
        constexpr StringView(Iter begin, End end)
            : _pointer{ toAddress(begin) }
            , _size{ end - begin }
        {
        }

        // 現状は、rangesをサポートしない。

        ~StringView() noexcept = default;

        constexpr StringView& operator=(const StringView&) noexcept = default;
        StringView& operator=(StringView&&) = default;

        [[nodiscard]] constexpr const_iterator begin() const noexcept
        {
            return cbegin();
        }

        [[nodiscard]] constexpr const_iterator end() const noexcept
        {
            return cend();
        }

        [[nodiscard]] constexpr const_iterator cbegin() const noexcept
        {
            ZEN_EXPECTS(!isEmpty());
            return _pointer;
        }

        [[nodiscard]] constexpr const_iterator cend() const noexcept
        {
            ZEN_EXPECTS(!isEmpty());
            return _pointer + _size;
        }

        [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        [[nodiscard]] constexpr size_type getSize() const noexcept
        {
            return _size;
        }

        [[nodiscard]] constexpr size_type getMaxSize() const noexcept
        {
            return (std::numeric_limits<size_type>::max)();
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

        // atはサポートしない。[]で常にレンジチェックを行う。ただし、リリース時はパフォーマンスのために、ノーチェック。

        [[nodiscard]] constexpr const_reference front() const
        {
            ZEN_EXPECTS(!isEmpty());
            return *_pointer;
        }

        [[nodiscard]] constexpr const_reference back() const
        {
            ZEN_EXPECTS(!isEmpty());
            return _pointer[_size - 1];
        }

        [[nodiscard]] constexpr const_pointer getData() const noexcept
        {
            return _pointer;
        }

        constexpr void removePrefix(size_type n) noexcept
        {
            ZEN_EXPECTS(n <= _size);
            _pointer += n;
            _size -= n;
        }

        constexpr void removeSuffix(size_type n) noexcept
        {
            ZEN_EXPECTS(n <= _size);
            _size -= n;
        }

        constexpr void swap(StringView& other) noexcept
        {
            const StringView temp{ other };
            other = *this;
            *this = temp;
        }

        size_type copy(char_type* s, size_type n, size_type pos = 0) const
        {
            ZEN_EXPECTS(pos <= _size);
            if (pos > _size) [[unlikely]]
            {
                return 0;
            }
            const size_type diff{ _size - pos };
            const size_type rlen{ n > diff ? diff : n };
            detail::StringHelper::copy(s, _pointer + pos, rlen);

            return rlen;
        }

        [[nodiscard]] constexpr StringView substr(size_type pos = 0, size_type n = npos) const
        {
            const size_type diff{ _size - pos };
            const size_type rlen{ n > diff ? diff : n };
            return StringView{ _pointer + pos, rlen };
        }

        [[nodiscard]] constexpr int compare(StringView other) const noexcept
        {
            const size_type rlen{ getSize() > other.getSize() ? other.getSize() : getSize() };

            const int result{ detail::StringHelper::compare(getData(), other.getData(), rlen) };
            if (result != 0)
            {
                return result;
            }

            if (getSize() == other.getSize())
            {
                return 0;
            }
            else if (getSize() < other.getSize())
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }

        [[nodiscard]] constexpr int compare(size_type pos1, size_type n1, StringView other) const
        {
            return substr(pos1, n1).compare(other);
        }

        [[nodiscard]] constexpr int compare(size_type pos1, size_type n1, StringView sv, size_type pos2, size_type n2) const
        {
            return substr(pos1, n1).compare(sv.substr(pos2, n2));
        }

        [[nodiscard]] constexpr int compare(const char_type* s) const
        {
            return compare(StringView{ s });
        }

        [[nodiscard]] constexpr int compare(size_type pos1, size_type n1, const char_type* s) const
        {
            return substr(pos1, n1).compare(StringView{ s });
        }

        [[nodiscard]] constexpr int compare(size_type pos1, size_type n1, const char_type* s, size_type n2) const
        {
            return substr(pos1, n1).compare(StringView{ s, n2 });
        }

        [[nodiscard]] constexpr bool startsWith(StringView other) const noexcept
        {
            return getSize() >= other.getSize() && compare(0, other.getSize(), other) == 0;
        }

        [[nodiscard]] constexpr bool startsWith(char_type x) const noexcept
        {
            return !isEmpty() && detail::StringHelper::eq(front(), x);
        }

        [[nodiscard]] constexpr bool startsWith(const char_type* x) const
        {
            return startsWith(StringView(x));
        }

        [[nodiscard]] constexpr bool endsWith(StringView other) const noexcept
        {
            return getSize() >= other.getSize() && compare(getSize() - other.getSize(), npos, other) == 0;
        }

        [[nodiscard]] constexpr bool endsWith(char_type x) const noexcept
        {
            return !isEmpty() && detail::StringHelper::eq(back(), x);
        }

        [[nodiscard]] constexpr bool endsWith(const char_type* x) const
        {
            return endsWith(StringView{ x });
        }

        [[nodiscard]] constexpr size_type find(StringView other, size_type pos = 0) const noexcept
        {
            return detail::StringHelper::find(_pointer, _size, pos, other._pointer, other._size);
        }

        [[nodiscard]] constexpr size_type find(char_type c, size_type pos = 0) const noexcept
        {
            return find(StringView{ &c, 1 }, pos);
        }

        [[nodiscard]] constexpr size_type find(const char_type* s, size_type pos, size_type n) const
        {
            return find(StringView{ s, n }, pos);
        }

        [[nodiscard]] constexpr size_type find(const char_type* s, size_type pos = 0) const
        {
            return find(StringView{ s }, pos);
        }

        [[nodiscard]] constexpr size_type rfind(StringView other, size_type pos = npos) const noexcept
        {
            return detail::StringHelper::rfind(_pointer, _size, pos, other._pointer, other._size);
        }

        [[nodiscard]] constexpr size_type rfind(char_type c, size_type pos = npos) const noexcept
        {
            return rfind(StringView{ &c, 1 }, pos);
        }

        [[nodiscard]] constexpr size_type rfind(const char_type* s, size_type pos, size_type n) const
        {
            return rfind(StringView{ s, n }, pos);
        }

        [[nodiscard]] constexpr size_type rfind(const char_type* s, size_type pos = npos) const
        {
            return rfind(StringView{ s }, pos);
        }

        [[nodiscard]] constexpr size_type findFirstOf(StringView other, size_type pos = 0) const noexcept
        {
            return detail::StringHelper::findFirstOf(_pointer, _size, pos, other._pointer, other._size);
        }

        [[nodiscard]] constexpr size_type findFirstOf(char_type c, size_type pos = 0) const noexcept
        {
            return findFirstOf(StringView{ &c, 1 }, pos);
        }

        [[nodiscard]] constexpr size_type findFirstOf(const char_type* s, size_type pos, size_type n) const
        {
            return findFirstOf(StringView{ s, n }, pos);
        }

        [[nodiscard]] constexpr size_type findFirstOf(const char_type* s, size_type pos = 0) const
        {
            return findFirstOf(StringView{ s }, pos);
        }

        [[nodiscard]] constexpr size_type findLastOf(StringView other, size_type pos = npos) const noexcept
        {
            return detail::StringHelper::findLastOf(_pointer, _size, pos, other._pointer, other._size);
        }

        [[nodiscard]] constexpr size_type findLastOf(char_type c, size_type pos = npos) const noexcept
        {
            return findLastOf(StringView{ &c, 1 }, pos);
        }

        [[nodiscard]] constexpr size_type findLastOf(const char_type* s, size_type pos, size_type n) const
        {
            return findLastOf(StringView{ s, n }, pos);
        }

        [[nodiscard]] constexpr size_type findLastOf(const char_type* s, size_type pos = npos) const
        {
            return findLastOf(StringView{ s }, pos);
        }

        [[nodiscard]] constexpr size_type findFirstNotOf(StringView other, size_type pos = 0) const noexcept
        {
            return detail::StringHelper::findFirstNotOf(_pointer, _size, pos, other._pointer, other._size);
        }

        [[nodiscard]] constexpr size_type findFirstNotOf(char_type c, size_type pos = 0) const noexcept
        {
            return findFirstNotOf(StringView{ &c, 1 }, pos);
        }

        [[nodiscard]] constexpr size_type findFirstNotOf(const char_type* s, size_type pos, size_type n) const
        {
            return findFirstNotOf(StringView{ s, n }, pos);
        }

        [[nodiscard]] constexpr size_type findFirstNotOf(const char_type* s, size_type pos = 0) const
        {
            return findFirstNotOf(StringView{ s }, pos);
        }

        [[nodiscard]] constexpr size_type findLastNotOf(StringView other, size_type pos = npos) const noexcept
        {
            return detail::StringHelper::findLastNotOf(_pointer, _size, pos, other._pointer, other._size);
        }

        [[nodiscard]] constexpr size_type findLastNotOf(char_type c, size_type pos = npos) const noexcept
        {
            return findLastNotOf(StringView{ &c, 1 }, pos);
        }

        [[nodiscard]] constexpr size_type findLastNotOf(const char_type* s, size_type pos, size_type n) const
        {
            return findLastNotOf(StringView{ s, n }, pos);
        }

        [[nodiscard]] constexpr size_type findLastNotOf(const char_type* s, size_type pos = npos) const
        {
            return findLastNotOf(StringView{ s }, pos);
        }

        [[nodiscard]] constexpr bool contains(StringView x) const noexcept
        {
            return find(x) != npos;
        }

        [[nodiscard]] constexpr bool contains(char_type x) const noexcept
        {
            return find(x) != npos;
        }

        [[nodiscard]] constexpr bool contains(const char_type* x) const
        {
            return find(x) != npos;
        }

    protected:
        const_pointer _pointer;
        size_type _size;
    };

    [[nodiscard]] inline constexpr bool operator==(StringView a, StringView b) noexcept
    {
        if (a.getSize() != b.getSize())
        {
            return false;
        }
        return a.compare(b) == 0;
    }

    [[nodiscard]] inline constexpr bool operator!=(StringView a, StringView b) noexcept
    {
        return !(a == b);
    }

    [[nodiscard]] inline constexpr bool operator<(StringView a, StringView b) noexcept
    {
        return a.compare(b) < 0;
    }

    [[nodiscard]] inline constexpr bool operator<=(StringView a, StringView b) noexcept
    {
        return a.compare(b) <= 0;
    }

    [[nodiscard]] inline constexpr bool operator>(StringView a, StringView b) noexcept
    {
        return a.compare(b) > 0;
    }

    [[nodiscard]] inline constexpr bool operator>=(StringView a, StringView b) noexcept
    {
        return a.compare(b) >= 0;
    }

    inline namespace literals
    {
        inline namespace string_view_literals
        {
            [[nodiscard]] constexpr StringView operator""_strv(const char16_t* str, std::size_t length) noexcept
            {
                return StringView{ str, length };
            }
        }
    }
}
