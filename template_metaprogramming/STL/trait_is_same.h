#pragma once

#include "trait_bool_constant.h"

template <typename T1, typename T2>
struct is_same : public false_type
{
};

template <typename T>
struct is_same<T, T> : public true_type
{
};

// the _v helper
template <typename T1, typename T2>
inline constexpr bool is_same_v = is_same<T1, T2>::value;

static_assert(not is_same_v<int, double>);
static_assert(not is_same_v<int, const int>);
static_assert(is_same_v<int, int>);
