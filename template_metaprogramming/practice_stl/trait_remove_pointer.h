#pragma once

#include "trait_is_same.h"
#include "trait_type_identity.h"

template <typename T>
struct remove_pointer : public type_identity<T>
{
};

template <typename T>
struct remove_pointer<T*> : public type_identity<T>
{
};

template <typename T>
struct remove_pointer<T* const> : public type_identity<T>
{
};

template <typename T>
struct remove_pointer<T* volatile> : public type_identity<T>
{
};

template <typename T>
struct remove_pointer<T* const volatile> : public type_identity<T>
{
};

// the _t helper
template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

static_assert(is_same_v<int, remove_pointer_t<int>>);
static_assert(is_same_v<int, remove_pointer_t<int*>>);
static_assert(is_same_v<int, remove_pointer_t<int* const>>);
static_assert(is_same_v<int, remove_pointer_t<int* volatile>>);
static_assert(is_same_v<int, remove_pointer_t<int* const volatile>>);
