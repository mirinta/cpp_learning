#pragma once

#include "trait_type_identity.h"

/**
 * If T is an array type, remove its first dimension.
 * Examples:
 * - int[] -> int
 * - int[][3] -> int[3]
 */
template <typename T>
struct remove_extent : public type_identity<T>
{
};

template <typename T>
struct remove_extent<T[]> : public type_identity<T>
{
};

template <typename T, size_t N>
struct remove_extent<T[N]> : public type_identity<T>
{
};

// the _t helper
template <typename T>
using remove_extent_t = typename remove_extent<T>::type;
