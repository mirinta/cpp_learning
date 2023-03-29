#pragma once

#include "trait_bool_constant.h"

template <typename T>
struct is_array : public false_type
{
};

template <typename T>
struct is_array<T[]> : public true_type
{
};

template <typename T, size_t N>
struct is_array<T[N]> : public true_type
{
};

// the _v helper
template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

static_assert(not is_array_v<int>);
static_assert(is_array_v<int[]>);
static_assert(is_array_v<int[1]>);
static_assert(is_array_v<int[][2]>);
static_assert(is_array_v<int[3][4]>);
