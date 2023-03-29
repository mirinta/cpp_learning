#pragma once

#include "trait_bool_constant.h"

/**
 * @brief Check if T is a pointer.
 * @note Pointers may have low-level cv-qualifiers.
 */
template <typename T>
struct is_pointer : public false_type
{
};

template <typename T>
struct is_pointer<T*> : public true_type
{
};

template <typename T>
struct is_pointer<T* const> : public true_type
{
};

template <typename T>
struct is_pointer<T* volatile> : public true_type
{
};

template <typename T>
struct is_pointer<T* const volatile> : public true_type
{
};

// the _v helper
template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

static_assert(not is_pointer_v<int>);
static_assert(is_pointer_v<int*>);
static_assert(is_pointer_v<int* const>);
static_assert(is_pointer_v<int* volatile>);
static_assert(is_pointer_v<int* const volatile>);
