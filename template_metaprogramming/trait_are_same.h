#pragma once

#include <type_traits>

/**
 * @brief Check the types are exact the same.
 * @note
 * 1. It is a false type if the list is empty
 * 2. It is a true type if the list has only one type.
 * 3. std::conjunction<>::value = true.
 */
template <typename... Ts>
struct are_same : std::false_type
{
};

template <typename T, typename... Ts>
struct are_same<T, Ts...> : public std::conjunction<std::is_same<T, Ts>...>
{
};

template <typename... Ts>
inline constexpr auto are_same_v = are_same<Ts...>::value;

namespace test_are_same {
static_assert(not are_same_v<>);
static_assert(not are_same_v<int, int, char>);
static_assert(not are_same_v<int, const int>);
static_assert(are_same_v<int>);
static_assert(are_same_v<int, int>);
} // namespace test_are_same
