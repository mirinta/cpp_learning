#pragma once

#include <concepts>
#include <type_traits>

/**
 * Compile-time operations of @see type_list:
 *  1. size of the list;
 *  2. append given type T;
 *  3. prepend given type T;
 *  4. merge given lists;
 *  6. reverse the order of the types;
 *  7. get the type at given index I;
 *  8. get the index of the first occurrence of given type T in the list (-1 if not found);
 *  9. check if the list contains given type T;
 * 10. remove the type at given index I;
 * 11. replace the type at given index I with given type T;
 *
 * TODO: swap? extract? enumera? sort?
 *
 * Reference: https://github.com/lipk/cpp-typelist/blob/master/typelist.hpp
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
constexpr auto is_type_list_v = is_type_list<T>::value;

template <typename... Ts>
struct type_count : public std::integral_constant<size_t, sizeof...(Ts)>
{
};

template <typename... Ts>
constexpr auto type_count_v = type_count<Ts...>::value;

template <typename L1, typename... L2>
struct merge_lists;

template <typename L1, typename... L2>
using merge_lists_t = typename merge_lists<L1, L2...>::type;

template <typename... Ts, typename... Us, typename... Ls>
struct merge_lists<type_list<Ts...>, type_list<Us...>, Ls...>
{
    using type = merge_lists_t<type_list<Ts..., Us...>, Ls...>;
};

template <typename... Ts>
struct merge_lists<type_list<Ts...>>
{
    using type = type_list<Ts...>;
};

template <size_t I, typename... Ts>
struct get_nth_type;

template <size_t I, typename... Ts>
using get_nth_type_t = typename get_nth_type<I, Ts...>::type;

template <typename Head, typename... Tail>
struct get_nth_type<0, Head, Tail...>
{
    using type = Head;
};

template <size_t I, typename Head, typename... Tail>
struct get_nth_type<I, Head, Tail...>
{
    using type = get_nth_type_t<I - 1, Tail...>;
};

template <typename T, size_t I, typename... Ts>
struct type_index;

template <typename T, size_t I, typename... Ts>
constexpr auto type_index_v = type_index<T, I, Ts...>::value;

template <typename T, size_t I>
struct type_index<T, I> : public std::integral_constant<int64_t, -1>
{
};

template <typename T, size_t I, typename... Ts>
struct type_index<T, I, T, Ts...> : public std::integral_constant<int64_t, I>
{
};

template <typename T, size_t I, typename Head, typename... Tail>
struct type_index<T, I, Head, Tail...>
    : public std::integral_constant<int64_t, type_index_v<T, I + 1, Tail...>>
{
};

template <size_t I, typename... Ts>
struct remove_nth_type;

template <size_t I, typename... Ts>
using remove_nth_type_t = typename remove_nth_type<I, Ts...>::type;

template <size_t I, typename Head, typename... Tail>
struct remove_nth_type<I, Head, Tail...>
{
    using type = merge_lists_t<type_list<Head>, remove_nth_type_t<I - 1, Tail...>>;
};

template <typename Head, typename... Tail>
struct remove_nth_type<0, Head, Tail...>
{
    using type = type_list<Tail...>;
};

template <size_t I, typename T, typename... Ts>
struct replace_nth_type;

template <size_t I, typename T, typename... Ts>
using replace_nth_type_t = typename replace_nth_type<I, T, Ts...>::type;

template <size_t I, typename T, typename Head, typename... Tail>
struct replace_nth_type<I, T, Head, Tail...>
{
    using type = merge_lists_t<type_list<Head>, replace_nth_type_t<I - 1, T, Tail...>>;
};

template <typename T, typename Head, typename... Tail>
struct replace_nth_type<0, T, Head, Tail...>
{
    using type = type_list<T, Tail...>;
};

template <typename... Ts>
struct reverse_list
{
    using type = type_list<>;
};

template <typename... Ts>
using reverse_list_t = typename reverse_list<Ts...>::type;

template <typename Head, typename... Tail>
struct reverse_list<Head, Tail...>
{
    using type = merge_lists_t<reverse_list_t<Tail...>, type_list<Head>>;
};
} // namespace impl

template <typename T>
concept TypeList = impl::is_type_list_v<T>;

template <typename L, size_t Is>
concept ValidIndex = TypeList<L> && (Is < L::size);

template <typename... Ts>
struct type_list
{
    static constexpr auto size = impl::type_count_v<Ts...>;

    template <typename T>
    using append = type_list<Ts..., T>;

    template <typename T>
    using prepend = type_list<T, Ts...>;

    template <TypeList L, TypeList... Ls>
    using merge = impl::merge_lists_t<type_list, L, Ls...>;

    using reverse = impl::reverse_list_t<Ts...>;

    template <size_t I>
    requires ValidIndex<type_list, I> using at = impl::get_nth_type_t<I, Ts...>;

    template <typename T>
    static constexpr auto indexOf = impl::type_index_v<T, 0, Ts...>;

    template <typename T>
    static constexpr auto contains = indexOf<T> >= 0;

    template <size_t I>
    requires ValidIndex<type_list, I> using removeAt = impl::remove_nth_type_t<I, Ts...>;

    template <size_t I, typename T>
    requires ValidIndex<type_list, I> using replaceAt = impl::replace_nth_type_t<I, T, Ts...>;
};

/// test
using L0 = type_list<>;
using L1 = type_list<int>;
using L2 = type_list<long, double>;

static_assert(0 == L0::size);
static_assert(1 == L1::size);
static_assert(2 == L2::size);

static_assert(std::is_same_v<L1, L0::append<int>>);
static_assert(std::is_same_v<L1, L0::prepend<int>>);
static_assert(std::is_same_v<type_list<int, long, double>, L2::prepend<int>>);
static_assert(std::is_same_v<type_list<long, double, int>, L2::append<int>>);

static_assert(std::is_same_v<L0, L0::merge<L0>>);
static_assert(std::is_same_v<L1, L0::merge<L0, L0, L1>>);
static_assert(std::is_same_v<type_list<int, int, long, double>, L0::merge<L1, L1, L2>>);

static_assert(std::is_same_v<int, L1::at<0>>);
static_assert(std::is_same_v<double, L2::at<1>>);

static_assert(-1 == L0::indexOf<int>);
static_assert(-1 == L1::indexOf<double>);
static_assert(0 == L1::indexOf<int>);
static_assert(1 == L2::indexOf<double>);
static_assert(2 == type_list<int, long, double>::indexOf<double>);

static_assert(not L0::contains<int>);
static_assert(not L1::contains<double>);
static_assert(L1::contains<int>);
static_assert(L2::contains<double>);
static_assert(type_list<int, long, double>::contains<double>);

static_assert(std::is_same_v<L0, L1::removeAt<0>>);
static_assert(std::is_same_v<L2, L2::merge<L1>::removeAt<L2::size>>);

static_assert(std::is_same_v<L2::removeAt<1>, L1::replaceAt<0, long>>);
static_assert(std::is_same_v<type_list<long, size_t>, L2::replaceAt<1, size_t>>);

static_assert(std::is_same_v<L0, L0::reverse>);
static_assert(std::is_same_v<L1, L1::reverse>);
static_assert(std::is_same_v<L2, L2::reverse::reverse>);
