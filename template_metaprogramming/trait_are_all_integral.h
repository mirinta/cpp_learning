#pragma once

#include <type_traits>

/**
 * @brief Check the arguments are all integral types.
 * @note
 * 1. It is a false type if the list is empty.
 * 2. std::conjunction<>::value = true.
 */
template <typename... Ts>
struct are_all_integral : public std::conjunction<std::is_integral<Ts>...>
{
};

// partial specialization, the list is empty
template <>
struct are_all_integral<> : public std::false_type
{
};

template <typename... Ts>
inline constexpr bool are_all_integral_v = are_all_integral<Ts...>::value;

namespace test_are_all_integral {
static_assert(not are_all_integral_v<>);
static_assert(not are_all_integral_v<int, double>);
static_assert(are_all_integral_v<int>);
static_assert(are_all_integral_v<const int>);
static_assert(are_all_integral_v<char, int, unsigned, bool>);
} // namespace test_are_all_integral
