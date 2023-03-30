#pragma once

#include <type_traits>

/**
 * If T is an array type, provides the member constant value equal to the number of dimensions of
 * the array. For any other type, value is 0.
 */
template <typename T>
struct rank : public std::integral_constant<size_t, 0>
{
};

template <typename T>
struct rank<T[]> : public std::integral_constant<size_t, 1 + rank<T>::value>
{
};

template <typename T, size_t N>
struct rank<T[N]> : public std::integral_constant<size_t, 1 + rank<T>::value>
{
};

// the _v helper
template <typename T>
inline constexpr auto rank_v = rank<T>::value;

static_assert(0 == rank_v<int>);
static_assert(1 == rank_v<int[]>);
static_assert(1 == rank_v<int[1]>);
static_assert(2 == rank_v<int[][2]>);
static_assert(3 == rank_v<int[][2][3]>);
static_assert(4 == rank_v<int[][2][3][4]>);
