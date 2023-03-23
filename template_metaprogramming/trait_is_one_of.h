#pragma once

#include <type_traits>

/**
 * @brief Check T is in the list.
 * @note It is a false type if the list is empty.
 * std::disjunction<>::value = false.
 */
template <typename T, typename... Ts>
struct is_one_of : public std::disjunction<std::is_same<T, Ts>...>
{
};

template <typename T, typename... Ts>
inline constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;

/// a straightforward version:
namespace version2 {
// primary template
template <typename T, typename... T0toN>
struct is_one_of;

// partial specialization, the list is empty
template <typename T>
struct is_one_of<T> : public std::false_type
{
};

// partial specializatin, T is at the first of the list
template <typename T, typename... T1toN>
struct is_one_of<T, T, T1toN...> : public std::true_type
{
};

// partial specialization, T is not at the first of the list, resursively call the primary template
template <typename T, typename T0, typename... T1toN>
struct is_one_of<T, T0, T1toN...> : public is_one_of<T, T1toN...>
{
};

template <typename T, typename... Ts>
inline constexpr bool is_one_of_v = is_one_of<T, Ts...>::value;
} // namespace version2

namespace test_is_on_of {
static_assert(not is_one_of_v<int>);
static_assert(not is_one_of_v<int, char, const int>);
static_assert(is_one_of_v<int, char, int, double>);
static_assert(not version2::is_one_of_v<int>);
static_assert(not version2::is_one_of_v<int, char, const int>);
static_assert(version2::is_one_of_v<int, char, int, double>);
}; // namespace test_is_on_of
