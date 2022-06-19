#pragma once


#define ZEN_IMPLEMENT_FLAG_OPERATORS(T)                                                                                    \
inline constexpr T operator^(T a, T b)                                                                                    \
{                                                                                                                         \
	return static_cast<T>(static_cast<std::underlying_type<T>::type>(a) ^ static_cast<std::underlying_type<T>::type>(b)); \
}                                                                                                                         \
inline constexpr T operator&(T a, T b)                                                                                    \
{                                                                                                                         \
    return static_cast<T>(static_cast<std::underlying_type<T>::type>(a) & static_cast<std::underlying_type<T>::type>(b)); \
}                                                                                                                         \
inline constexpr T operator|(T a, T b)                                                                                    \
{                                                                                                                         \
    return static_cast<T>(static_cast<std::underlying_type<T>::type>(a) | static_cast<std::underlying_type<T>::type>(b)); \
}                                                                                                                         \
inline constexpr T& operator&=(T& a, T b)                                                                                 \
{                                                                                                                         \
    return a = (a & b);                                                                                                   \
}                                                                                                                         \
inline constexpr T& operator|=(T& a, T b)                                                                                 \
{                                                                                                                         \
    return a = (a | b);                                                                                                   \
}                                                                                                                         \
inline constexpr T& operator^=(T& a, T b)                                                                                 \
{                                                                                                                         \
    return a = (a ^ b);                                                                                                   \
}                                                                                                                         \
inline constexpr bool operator!(T a)                                                                                      \
{                                                                                                                         \
    return !static_cast<std::underlying_type<T>::type>(a);                                                                \
}                                                                                                                         \
inline constexpr T operator~(T a)                                                                                         \
{                                                                                                                         \
    return static_cast<T>(~static_cast<std::underlying_type<T>::type>(a));                                                \
}