#pragma once

#include <concepts>
#include <type_traits>

/**
 * compile-time operations:
 * 1. length
 * 2. append Ts...
 * 3. prepend Ts...
 * 4. get at given index
 * 6. has T
 * 7. index of T
 * 8. erase T
 * 9. replace T to U
 * 10. remove duplicates
 * 11. reverse
 * 12. enumerate?
 * 13. sort?
 * 14. merge elements of L2 to L1
 * https://github.com/lipk/cpp-typelist/blob/master/tests.cpp
 * https://github.com/Pseudomanifold/TypeSwitch/blob/master/type_list.hh
 * https://www.cs.fsu.edu/~lacher/lectures/Output/loki3/index.html?$$$slide01.html$$$
 */
template <typename...>
struct type_list;

namespace impl {
template <typename T>
struct is_type_list : public std::false_type
{
};

template <typename... Ts>
struct is_type_list<type_list<Ts...>> : public std::true_type
{
};

template <typename T>
concept TypeList = is_type_list<T>::value;

template <typename... Ts>
inline constexpr size_t type_count_v = sizeof...(Ts);

template <typename... Ts>
concept NonEmpty = type_count_v<Ts...> > 0;

template <size_t Index, typename... Ts>
concept ValidIndex = Index < type_count_v<Ts...>;

template <typename... Ts>
requires NonEmpty<Ts...> struct list_head;

template <typename T0, typename... Ts>
struct list_head<T0, Ts...> : public std::type_identity<T0>
{
};

template <typename... Ts>
using list_head_t = typename list_head<Ts...>::type;

template <typename... Ts>
requires NonEmpty<Ts...> struct list_tail;

template <typename T0, typename... Ts>
struct list_tail<T0, Ts...> : public std::type_identity<type_list<Ts...>>
{
};

template <typename... Ts>
using list_tail_t = typename list_tail<Ts...>::type;

template <typename, TypeList>
struct merge_lists;

template <typename... Ts, typename... Us>
struct merge_lists<type_list<Ts...>, type_list<Us...>>
{
    using type = type_list<Ts..., Us...>;
};

template <size_t Index, typename... Ts>
requires ValidIndex<Index, Ts...> struct get_type
{
    using type = typename list_tail_t<Ts...>::template get<Index - 1>;
};

template <typename... Ts>
struct get_type<0, Ts...>
{
    using type = list_head_t<Ts...>;
};

template <typename T, size_t Index, typename... Ts>
struct type_index : public std::integral_constant<int64_t, -1>
{
};

template <typename T, size_t Index, typename... Ts>
struct type_index<T, Index, T, Ts...> : public std::integral_constant<int64_t, Index>
{
};

template <typename T, size_t Index, typename T0, typename... Ts>
struct type_index<T, Index, T0, Ts...>
    : public std::integral_constant<int64_t, type_index<T, Index + 1, Ts...>::value>
{
};
} // namespace impl

template <typename... Ts>
struct type_list
{
    static constexpr auto length = impl::type_count_v<Ts...>;

    template <typename T>
    using append = type_list<Ts..., T>;

    template <typename T>
    using prepend = type_list<T, Ts...>;

    template <typename L>
    using merge = typename impl::merge_lists<type_list<Ts...>, L>::type;

    template <size_t Index>
    using get = typename impl::get_type<Index, Ts...>::type;

    template <typename T>
    static constexpr auto find = impl::type_index<T, 0, Ts...>::value;

    template <typename T>
    static constexpr auto has = find<T> >= 0;
};

/// test
using L0 = type_list<>;
using L1 = type_list<int>;
using L2 = type_list<long, double>;

static_assert(0 == L0::length);
static_assert(1 == L1::length);
static_assert(2 == L2::length);

static_assert(std::is_same_v<L1, L0::append<int>>);
static_assert(std::is_same_v<L1, L0::prepend<int>>);
static_assert(std::is_same_v<type_list<int, long, double>, L2::prepend<int>>);
static_assert(std::is_same_v<type_list<long, double, int>, L2::append<int>>);

static_assert(std::is_same_v<L0, L0::merge<L0>>);
static_assert(std::is_same_v<L1, L0::merge<L1>>);

static_assert(std::is_same_v<int, L1::get<0>>);
static_assert(std::is_same_v<double, L2::get<1>>);

static_assert(-1 == L0::find<int>);
static_assert(-1 == L1::find<double>);
static_assert(0 == L1::find<int>);
static_assert(1 == L2::find<double>);
static_assert(2 == type_list<int, long, double>::find<double>);

static_assert(not L0::has<int>);
static_assert(not L1::has<double>);
static_assert(L1::has<int>);
static_assert(L2::has<double>);
static_assert(type_list<int, long, double>::has<double>);
