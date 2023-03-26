#pragma once

#include <concepts>
#include <type_traits>

template <typename...>
struct List
{
    static constexpr size_t size = 0;
};

template <typename T0, typename... T1toN>
struct List<T0, T1toN...>
{
    using head_type = T0;
    using tail_type = List<T1toN...>;
    static constexpr size_t size = 1 + tail_type::size;
};

template <typename L>
using list_head_t = typename L::head_type;

template <typename L>
using list_tail_t = typename L::tail_type;

/// concatenate two Lists
template <typename L1, typename L2>
struct List_Concat;

template <typename... Ts, typename... Us>
struct List_Concat<List<Ts...>, List<Us...>>
{
    using type = List<Ts..., Us...>;
};

template <typename L1, typename L2>
using list_concat_t = typename List_Concat<L1, L2>::type;

/// select the type at given index
template <typename L, size_t Index>
requires(Index < L::size) struct List_Select
{
    using type = typename List_Select<list_tail_t<L>, Index - 1>::type;
};

template <typename L>
struct List_Select<L, 0>
{
    using type = list_head_t<L>;
};

template <typename L, size_t Index>
using list_select_t = typename List_Select<L, Index>::type;

/// extract first N types
template <typename L, size_t N>
requires(N <= L::size) struct List_FirstN
{
    using type =
        list_concat_t<List<list_head_t<L>>, typename List_FirstN<list_tail_t<L>, N - 1>::type>;
};

template <typename L>
struct List_FirstN<L, 0>
{
    using type = List<>;
};

template <typename L, size_t N>
using list_firstN_t = typename List_FirstN<L, N>::type;

/// test
using L0 = List<>;
using L1 = List<const int>;
using L2 = List<int, double>;
using L3 = List<const int, int, double>;
using L4 = List<const int, int>;

static_assert(0 == L0::size);
static_assert(1 == L1::size);
static_assert(2 == L2::size);
static_assert(3 == L3::size);

static_assert(std::is_same_v<L3, list_concat_t<L1, L2>>);
static_assert(std::is_same_v<int, list_select_t<L2, 0>>);
static_assert(std::is_same_v<double, list_select_t<L3, 2>>);
static_assert(std::is_same_v<L0, list_firstN_t<L0, 0>>);
static_assert(std::is_same_v<L4, list_firstN_t<L3, 2>>);
static_assert(std::is_same_v<L3, list_firstN_t<L3, 3>>);
