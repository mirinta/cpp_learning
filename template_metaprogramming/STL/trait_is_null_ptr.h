#pragma once

#include "trait_is_same.h"
#include "trait_nullptr_t.h"
#include "trait_remove_cv.h"

template <typename T>
struct is_null_pointer : public is_same<nullptr_t, remove_cv_t<T>>
{
};

// the _v helper
template <typename T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

static_assert(not is_null_pointer_v<int>);
static_assert(is_null_pointer_v<const decltype(nullptr)>);
static_assert(is_null_pointer_v<volatile decltype(nullptr)>);
static_assert(is_null_pointer_v<const volatile decltype(nullptr)>);
