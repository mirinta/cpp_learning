#pragma once
#include <tuple>
#include <type_traits>

template <typename...>
struct type_list
{
};

/**
 * is empty
 */
template <typename LIST>
struct is_empty : public std::false_type
{
};

template <template <typename...> class LIST>
struct is_empty<LIST<>> : public std::true_type
{
};

template <typename LIST>
inline constexpr auto is_empty_v = is_empty<LIST>::value;

/**
 * size
 */
template <typename LIST>
struct size;

template <template <typename...> class LIST, typename... T0toN>
struct size<LIST<T0toN...>> : public std::integral_constant<size_t, sizeof...(T0toN)>
{
};

template <typename LIST>
inline constexpr auto size_v = size<LIST>::value;

/**
 * front
 */
template <typename LIST>
struct front;

template <template <typename...> class LIST, typename T0, typename... T1toN>
struct front<LIST<T0, T1toN...>> : public std::type_identity<T0>
{
};

template <typename LIST>
using front_t = typename front<LIST>::type;

/**
 * pop front
 */
template <typename LIST>
struct pop_front;

template <template <typename...> class LIST>
struct pop_front<LIST<>> : std::type_identity<LIST<>>
{
};

template <template <typename...> class LIST, typename T0, typename... T1toN>
struct pop_front<LIST<T0, T1toN...>> : public std::type_identity<LIST<T1toN...>>
{
};

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

/**
 * back
 */
template <typename LIST>
struct back : public std::type_identity<typename back<pop_front_t<LIST>>::type>
{
};

template <template <typename...> class LIST, typename T0>
struct back<LIST<T0>> : public std::type_identity<T0>
{
};

template <typename LIST>
using back_t = typename back<LIST>::type;

/**
 * push back
 */
template <typename LIST, typename T>
struct push_back;

template <template <typename...> class LIST, typename... T0toN, typename T>
struct push_back<LIST<T0toN...>, T> : public std::type_identity<LIST<T0toN..., T>>
{
};

template <typename LIST, typename T>
using push_back_t = typename push_back<LIST, T>::type;

/**
 * pop back
 */
template <typename LIST, typename RET_LIST = type_list<>>
struct pop_back : public std::type_identity<type_list<>>
{
};

template <typename T0, typename T1, typename... T2toN, typename RET_LIST>
struct pop_back<type_list<T0, T1, T2toN...>, RET_LIST>
    : public pop_back<type_list<T1, T2toN...>, push_back_t<RET_LIST, T0>>
{
};

template <typename T0, typename RET_LIST>
struct pop_back<type_list<T0>, RET_LIST> : public std::type_identity<RET_LIST>
{
};

template <typename LIST>
using pop_back_t = typename pop_back<LIST>::type;

/**
 * at
 */
template <typename LIST, size_t I>
struct at : public std::type_identity<typename at<pop_front_t<LIST>, I - 1>::type>
{
};

template <typename LIST>
struct at<LIST, 0> : public std::type_identity<front_t<LIST>>
{
};

template <typename LIST, size_t I>
using at_t = typename at<LIST, I>::type;

/**
 * any of
 */
template <template <typename> class PREDICATE, typename LIST>
struct any_of;

template <template <typename> class PREDICATE, template <typename...> class LIST>
struct any_of<PREDICATE, LIST<>> : public std::false_type
{
};

template <template <typename> class PREDICATE, typename LIST>
struct any_of : public std::conditional_t<PREDICATE<front_t<LIST>>::value, std::true_type,
                                          any_of<PREDICATE, pop_front_t<LIST>>>
{
};

template <template <typename> class PREDICATE, typename LIST>
inline constexpr auto any_of_v = any_of<PREDICATE, LIST>::value;

/**
 * contains
 */
template <typename T>
struct same_as
{
    template <typename U>
    struct predicate : std::is_same<T, U>
    {
    };
};

template <typename SEARCH, typename LIST>
inline constexpr auto contains_v = any_of_v<same_as<SEARCH>::template predicate, LIST>;

/// test
using L0 = type_list<>;
using L1 = type_list<int>;
using L2 = type_list<int, double>;

static_assert(std::is_same_v<L0, pop_back_t<L1>>);
static_assert(std::is_same_v<L1, pop_back_t<L2>>);
static_assert(std::is_same_v<at_t<L1, 0>, at_t<L2, 0>>);
static_assert(not contains_v<int, L0>);
static_assert(not contains_v<double, L1>);
static_assert(contains_v<int, L1>);
static_assert(contains_v<double, L2>);
