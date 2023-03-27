#pragma once

#include "trait_type_identity.h"
#include "trait_is_same.h"

/**
 * @brief Remove the top-level cv-qualifiers of T.
 */
template <typename T>
struct remove_cv : type_identity<T>
{
};

template <typename T>
struct remove_cv<const T> : type_identity<T>
{
};

template <typename T>
struct remove_cv<volatile T> : type_identity<T>
{
};

template <typename T>
struct remove_cv<const volatile T> : type_identity<T>
{
};

// the _t helper
template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

static_assert(is_same_v<int, remove_cv_t<int>>);
static_assert(is_same_v<int, remove_cv_t<const int>>);
static_assert(is_same_v<int, remove_cv_t<volatile int>>);
static_assert(is_same_v<int, remove_cv_t<const volatile int>>);
