#pragma once

#include <type_traits>

#include "trait_bool_constant.h"

/**
 * @brief Check if T is a class/struct.
 * @note It is a false type if T is an union.
 * Is it possible to implement is_union without using the STL?
 */
namespace is_class_detail {
template <typename T>
bool_constant<not std::is_union_v<T>> test(int T::*);

template <typename T>
false_type test(...);
} // namespace is_class_detail

template <typename T>
using is_class = decltype(is_class_detail::test<T>(nullptr));

// the _v helper
template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

class C;
struct S;
union U;
static_assert(not is_class_v<int>);
static_assert(not is_class_v<U>);
static_assert(is_class_v<S>);
static_assert(is_class_v<C>);
static_assert(is_class_v<const C>);
static_assert(is_class_v<volatile C>);
static_assert(is_class_v<const volatile C>);
