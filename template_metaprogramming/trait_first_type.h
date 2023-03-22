#pragma once

#include <type_traits>

/**
 * @brief Get the first type.
 * @note An empty list is not allowed.
 */
template <typename T, typename...>
struct first_type : public std::type_identity<T>
{
};

template <typename... Ts>
using first_type_t = typename first_type<Ts...>::type;

namespace test_first_type {
static_assert(std::is_same_v<int, first_type_t<int, char>>);
static_assert(not std::is_same_v<int, first_type_t<char, int>>);
} // namespace test_first_type
