#pragma once

#include "trait_is_same.h"
#include "trait_remove_cv.h"

template <typename T>
struct is_void : public is_same<void, remove_cv_t<T>>
{
};

// the _v helper
template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

static_assert(not is_void_v<int>);
static_assert(is_void_v<void>);
static_assert(is_void_v<const void>);
static_assert(is_void_v<volatile void>);
static_assert(is_void_v<const volatile void>);
