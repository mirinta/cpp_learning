#pragma once

#include <concepts>
#include <type_traits>

template <typename...>
struct type_list
{
};

template <typename Head, typename... Tail>
struct type_list<Head, Tail...>
{
    using head_type = Head;
    using tail_type = type_list<Tail...>;
};

/// metafunction: check if T is a type list
template <typename>
struct is_type_list : public std::false_type
{
};

template <typename... Ts>
struct is_type_list<type_list<Ts...>> : public std::true_type
{
};

template <typename T>
inline constexpr auto is_type_list_v = is_type_list<T>::value;

/// conpcet: satisfied if T is a type list
template <typename T>
concept IsTypeList = is_type_list_v<T>;

/// metafunction: count the number of types
template <IsTypeList>
struct type_list_count;

template <typename... Ts>
struct type_list_count<type_list<Ts...>>
{
    static constexpr size_t value = sizeof...(Ts);
};

template <typename L>
inline constexpr auto type_list_count_v = type_list_count<L>::value;

/// concept: satisfied if T a non-emplty type list
template <typename L>
concept TypeListNotEmpty = IsTypeList<L>&& type_list_count_v<L> > 0;

/// metafunction: get the first type of a non-empty type list
template <TypeListNotEmpty L>
using type_list_head_t = typename L::head_type;

/// metafunction: get the remaining type list except for the first one
template <TypeListNotEmpty L>
using type_list_tail_t = typename L::tail_type;

/// metafunction: append all elements of TypeList2 to TypeList1
template <IsTypeList, IsTypeList>
struct append_type_list;

template <typename... T1s, typename... T2s>
struct append_type_list<type_list<T1s...>, type_list<T2s...>>
{
    using type = type_list<T1s..., T2s...>;
};

template <typename L1, typename L2>
using append_type_list_t = typename append_type_list<L1, L2>::type;

/// metafunction: concatenate multiple TypeLists
template <IsTypeList...>
struct concat_type_lists
{
    using type = type_list<>;
};

template <typename L1>
struct concat_type_lists<L1>
{
    using type = L1;
};

template <typename L1, typename L2, typename... Ls>
struct concat_type_lists<L1, L2, Ls...>
{
    using type = typename concat_type_lists<append_type_list_t<L1, L2>, Ls...>::type;
};

template <typename... Ls>
using concat_type_lists_t = typename concat_type_lists<Ls...>::type;

/// metafunction: get the type at given index
template <TypeListNotEmpty L, size_t Index>
requires(Index < type_list_count_v<L>) struct get_type
{
    using type = typename get_type<type_list_tail_t<L>, Index - 1>::type;
};

template <typename L>
struct get_type<L, 0>
{
    using type = type_list_head_t<L>;
};

template <typename L, size_t Index>
using get_type_t = typename get_type<L, Index>::type;

/// metafunction: get the first N types
template <IsTypeList L, size_t N>
requires(type_list_count_v<L> >= N) struct first_n_types
{
    using type = append_type_list_t<type_list<type_list_head_t<L>>,
                                    typename first_n_types<type_list_tail_t<L>, N - 1>::type>;
};

template <typename L>
struct first_n_types<L, 0>
{
    using type = type_list<>;
};

template <typename L, size_t N>
using first_n_types_t = typename first_n_types<L, N>::type;

/// test
using L0 = type_list<>;
using L1 = type_list<const int>;
using L2 = type_list<int, double>;
using L3 = type_list<const int, int, double>;

static_assert(not IsTypeList<int>);
static_assert(IsTypeList<L0>);

static_assert(0 == type_list_count_v<L0>);
static_assert(1 == type_list_count_v<L1>);
static_assert(2 == type_list_count_v<L2>);
static_assert(3 == type_list_count_v<L3>);

static_assert(std::is_same_v<L0, append_type_list_t<L0, L0>>);
static_assert(std::is_same_v<L1, append_type_list_t<L0, L1>>);
static_assert(std::is_same_v<L3, append_type_list_t<L1, L2>>);

static_assert(std::is_same_v<L0, concat_type_lists_t<>>);
static_assert(std::is_same_v<L0, concat_type_lists_t<L0>>);
static_assert(std::is_same_v<L3, concat_type_lists_t<L0, L1, L2>>);

static_assert(std::is_same_v<const int, get_type_t<L1, 0>>);
static_assert(std::is_same_v<get_type_t<L2, 1>, get_type_t<L3, 2>>);

static_assert(std::is_same_v<L0, first_n_types_t<L0, 0>>);
static_assert(std::is_same_v<L1, first_n_types_t<L3, 1>>);
static_assert(std::is_same_v<type_list<const int, int>, first_n_types_t<L3, 2>>);
