#pragma once

#include <type_traits>

/**
 * @brief Count the number of types.
 */
template <typename... Ts>
using arguments_count = std::integral_constant<size_t, sizeof...(Ts)>;

template <typename... Ts>
inline constexpr auto arguments_count_v = arguments_count<Ts...>::value;

/// a straightforward version:
namespace version2 {
template <typename... T0toN>
struct arguments_count : public std::integral_constant<size_t, 0>
{
};

template <typename T, typename... T1toN>
struct arguments_count<T, T1toN...>
    : public std::integral_constant<size_t, 1 + arguments_count<T1toN...>::value>
{
};

template <typename... T0toN>
inline constexpr auto arguments_count_v = arguments_count<T0toN...>::value;
} // namespace version2

namespace test_arguments_count {
static_assert(arguments_count_v<> == 0);
static_assert(arguments_count_v<int> == 1);
static_assert(arguments_count_v<int, int, double> == 3);
static_assert(version2::arguments_count_v<> == 0);
static_assert(version2::arguments_count_v<int> == 1);
static_assert(version2::arguments_count_v<int, int, double> == 3);
} // namespace test_arguments_count
