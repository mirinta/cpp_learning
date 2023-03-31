#pragma once

#include <type_traits>

/// SIZE
template <typename LIST>
struct size;

template <template <typename...> class LIST, typename... T0toN>
struct size<LIST<T0toN...>> : public std::integral_constant<size_t, sizeof...(T0toN)>
{
};

template <typename LIST>
inline constexpr auto size_v = size<LIST>::value;

/// FRONT
template <typename LIST>
struct front;

template <template <typename...> class LIST, typename T0, typename... T1toN>
struct front<LIST<T0, T1toN...>> : public std::type_identity<T0>
{
};

template <typename LIST>
using front_t = typename front<LIST>::type;

/// POP_FRONT
template <typename LIST>
struct pop_front;

template <template <typename...> class LIST>
struct pop_front<LIST<>> : public std::type_identity<LIST<>>
{
};

template <template <typename...> class LIST, typename T0, typename... T1toN>
struct pop_front<LIST<T0, T1toN...>> : public std::type_identity<LIST<T1toN...>>
{
};

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;
