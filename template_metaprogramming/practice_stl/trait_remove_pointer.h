#pragma once

#include <type_traits>

template <typename T>
struct remove_pointer : public std::type_identity<T>
{
};

template <typename T>
struct remove_pointer<T*> : public std::type_identity<T>
{
};

template <typename T>
struct remove_pointer<T* const> : public std::type_identity<T>
{
};

template <typename T>
struct remove_pointer<T* volatile> : public std::type_identity<T>
{
};

template <typename T>
struct remove_pointer<T* const volatile> : public std::type_identity<T>
{
};

// the _t helper
template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

static_assert(std::is_same_v<int, remove_pointer_t<int>>);
static_assert(std::is_same_v<int, remove_pointer_t<int*>>);
static_assert(std::is_same_v<int, remove_pointer_t<int* const>>);
static_assert(std::is_same_v<int, remove_pointer_t<int* volatile>>);
static_assert(std::is_same_v<int, remove_pointer_t<int* const volatile>>);