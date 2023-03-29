#pragma once

#include <concepts>
#include <type_traits>

template <typename T, typename... Ts>
concept SameAsAny = (... or std::same_as<T, Ts>);
/// equivalent:
/// concept SameAsAny = std::disjunction_v<std::is_same<T, Ts>...>;

static_assert(not SameAsAny<int>);
static_assert(not SameAsAny<int, char, double>);
static_assert(not SameAsAny<int, char, const int>);
static_assert(SameAsAny<int, char, int>);
