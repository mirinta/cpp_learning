#pragma once

#include "trait_type_identity.h"

template <bool condition, typename T, typename F>
struct conditional : public type_identity<T>
{
};

template <typename T, typename F>
struct conditional<false, T, F> : public type_identity<F>
{
};

// the _t helper
template <bool condition, typename T, typename F>
using conditional_t = typename conditional<condition, T, F>::type;
