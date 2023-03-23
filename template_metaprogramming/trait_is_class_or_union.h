#pragma once

#include <type_traits>

/**
 * @brief Check if T is class/struct or union.
 */
template <typename T, typename = void>
struct is_class_or_union : public std::false_type
{
};

template <typename T>
struct is_class_or_union<T, std::void_t<int T::*>> : public std::true_type
{
};

template <typename T>
inline constexpr bool is_class_or_union_v = is_class_or_union<T>::value;

/// STL version
namespace version2 {
template <typename T>
inline constexpr bool is_class_or_union_v = std::is_union_v<T> || std::is_class_v<T>;

template <typename T>
struct is_class_or_union : public std::bool_constant<is_class_or_union_v<T>>
{
};
}; // namespace version2

/// function overloading version
namespace version3 {
template <typename T>
std::true_type is_class_or_union_impl(int T::*);

template <typename T>
std::false_type is_class_or_union_impl(...);

template <typename T>
using is_class_or_union = decltype(is_class_or_union_impl<T>(nullptr));

template <typename T>
inline constexpr bool is_class_or_union_v = is_class_or_union<T>::value;
} // namespace version3

namespace test_is_class_or_union {
class C;
struct S;
union U;
static_assert(not is_class_or_union_v<int>);
static_assert(is_class_or_union_v<C>);
static_assert(is_class_or_union_v<S>);
static_assert(is_class_or_union_v<U>);
static_assert(is_class_or_union_v<const U>);
static_assert(is_class_or_union_v<volatile U>);
static_assert(is_class_or_union_v<const volatile U>);
static_assert(not version2::is_class_or_union_v<int>);
static_assert(version2::is_class_or_union_v<C>);
static_assert(version2::is_class_or_union_v<S>);
static_assert(version2::is_class_or_union_v<U>);
static_assert(version2::is_class_or_union_v<const U>);
static_assert(version2::is_class_or_union_v<volatile U>);
static_assert(version2::is_class_or_union_v<const volatile U>);
static_assert(not version3::is_class_or_union_v<int>);
static_assert(version3::is_class_or_union_v<C>);
static_assert(version3::is_class_or_union_v<S>);
static_assert(version3::is_class_or_union_v<U>);
static_assert(version3::is_class_or_union_v<const U>);
static_assert(version3::is_class_or_union_v<volatile U>);
static_assert(version3::is_class_or_union_v<const volatile U>);
} // namespace test_is_class_or_union
