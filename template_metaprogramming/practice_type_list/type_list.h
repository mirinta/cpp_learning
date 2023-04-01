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
 * 12. swap the type at index I1 with the type at index I2;
 * 13. extract the types at given indices;
 * 14. remove duplicate types and retain their original order.
 *
 * TODO: enumerate? sort?
 *
 * Useful references:
 * https://github.com/lipk/cpp-typelist/blob/master/typelist.hpp
 * https://vittorioromeo.info/index/blog/cpp20_lambdas_compiletime_for.html
 */
template <typename...>
struct type_list;

namespace detail {
template <typename T>
struct is_type_list : public std::false_type
{
};

template <typename... Ts>
struct is_type_list<type_list<Ts...>> : public std::true_type
{
};

template <typename T>
inline constexpr auto is_type_list_v = is_type_list<T>::value;

template <typename... Ls>
struct merge
{
    using type = type_list<>;
};

template <typename... Ts>
struct merge<type_list<Ts...>>
{
    using type = type_list<Ts...>;
};

template <typename... Ts, typename... Us, typename... Ls>
struct merge<type_list<Ts...>, type_list<Us...>, Ls...>
{
    using type = typename merge<type_list<Ts..., Us...>, Ls...>::type;
};

template <typename L1, typename... L2>
using merge_t = typename merge<L1, L2...>::type;

template <size_t I, typename... Ts>
struct at;

template <typename Head, typename... Tail>
struct at<0, Head, Tail...>
{
    using type = Head;
};

template <size_t I, typename Head, typename... Tail>
struct at<I, Head, Tail...>
{
    using type = typename at<I - 1, Tail...>::type;
};

template <size_t I, typename... Ts>
using at_t = typename at<I, Ts...>::type;

template <typename T, size_t I, typename... Ts>
struct index_of : public std::integral_constant<int64_t, -1>
{
};

template <typename T, size_t I, typename... Ts>
struct index_of<T, I, T, Ts...> : public std::integral_constant<int64_t, I>
{
};

template <typename T, size_t I, typename Head, typename... Tail>
struct index_of<T, I, Head, Tail...>
    : public std::integral_constant<int64_t, index_of<T, I + 1, Tail...>::value>
{
};

template <typename T, size_t I, typename... Ts>
inline constexpr auto index_of_v = index_of<T, I, Ts...>::value;

template <typename T, typename... Ts>
inline constexpr auto contains_v = index_of_v<T, 0, Ts...> >= 0;

template <size_t I, typename... Ts>
struct remove_at;

template <size_t I, typename Head, typename... Tail>
struct remove_at<I, Head, Tail...>
{
    using type = merge_t<type_list<Head>, typename remove_at<I - 1, Tail...>::type>;
};

template <typename Head, typename... Tail>
struct remove_at<0, Head, Tail...>
{
    using type = type_list<Tail...>;
};

template <size_t I, typename... Ts>
using remove_at_t = typename remove_at<I, Ts...>::type;

template <size_t I, typename T, typename... Ts>
struct replace_at;

template <size_t I, typename T, typename Head, typename... Tail>
struct replace_at<I, T, Head, Tail...>
{
    using type = merge_t<type_list<Head>, typename replace_at<I - 1, T, Tail...>::type>;
};

template <typename T, typename Head, typename... Tail>
struct replace_at<0, T, Head, Tail...>
{
    using type = type_list<T, Tail...>;
};

template <size_t I, typename T, typename... Ts>
using replace_at_t = typename replace_at<I, T, Ts...>::type;

template <typename... Ts>
struct reverse
{
    using type = type_list<>;
};

template <typename Head, typename... Tail>
struct reverse<Head, Tail...>
{
    using type = merge_t<typename reverse<Tail...>::type, type_list<Head>>;
};

template <typename... Ts>
using reverse_t = typename reverse<Ts...>::type;

template <typename L, typename... Ts>
struct unique;

template <typename... Ts>
struct unique<type_list<Ts...>>
{
    using type = type_list<Ts...>;
};

template <typename... Ts, typename Head, typename... Tail>
struct unique<type_list<Ts...>, Head, Tail...>
{
    using type = std::conditional_t<type_list<Ts...>::template contains<Head>,
                                    typename unique<type_list<Ts...>, Tail...>::type,
                                    typename unique<type_list<Ts..., Head>, Tail...>::type>;
};

template <typename L, typename... Ts>
using unique_t = typename unique<L, Ts...>::type;

template <template <typename> class Predicate, typename... Ts>
struct any
{
};

template <template <typename> class Predicate, typename... Ts>
inline constexpr auto any_v = any<Predicate, Ts...>::value;
} // namespace detail

template <typename T>
concept TypeList = detail::is_type_list_v<T>;

template <typename L, size_t... Is>
concept ValidIndex = TypeList<L> && sizeof...(Is) > 0 && ((Is < L::size) && ...);

template <typename... Ts>
struct type_list
{
    static constexpr auto size = sizeof...(Ts);

    template <typename T>
    using append = type_list<Ts..., T>;

    template <typename T>
    using prepend = type_list<T, Ts...>;

    template <TypeList... Ls>
    using merge = detail::merge_t<type_list, Ls...>;

    template <size_t I>
    requires ValidIndex<type_list, I> using at = detail::at_t<I, Ts...>;

    template <size_t... Is>
    using slice = type_list<type_list::at<Is>...>;

    template <size_t I>
    requires ValidIndex<type_list, I> using removeAt = detail::remove_at_t<I, Ts...>;

    template <typename T>
    static constexpr auto indexOf = detail::index_of_v<T, 0, Ts...>;

    template <typename T>
    static constexpr auto contains = detail::contains_v<T, Ts...>;

    template <size_t I, typename T>
    requires ValidIndex<type_list, I> using replaceAt = detail::replace_at_t<I, T, Ts...>;

    template <size_t I1, size_t I2>
    using swapAt = typename replaceAt<I1, at<I2>>::template replaceAt<I2, at<I1>>;

    using reverse = detail::reverse_t<Ts...>;

    using unique = detail::unique_t<type_list<>, Ts...>;

    template <template <typename> class Predicate>
    static constexpr auto any = std::disjunction_v<Predicate<Ts>...>;
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

static_assert(std::is_same_v<L0, L0::merge<>>);
static_assert(std::is_same_v<L2::merge<L0>, L2::merge<>>);
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

static_assert(std::is_same_v<L1, L1::swapAt<0, 0>>);
static_assert(std::is_same_v<L2::reverse, L2::swapAt<0, 1>>);

static_assert(std::is_same_v<L0, L2::slice<>>);
static_assert(std::is_same_v<L1::merge<L1, L1>, L1::slice<0, 0, 0>>);
static_assert(std::is_same_v<L2::reverse, L2::slice<1, 0>>);

static_assert(std::is_same_v<L0, L0::unique>);
static_assert(std::is_same_v<L1, L1::merge<L1, L0>::unique>);
static_assert(std::is_same_v<L2, L2::merge<L2::reverse, L2>::unique>);

template <typename T>
struct same_as_predicate
{
    template <typename U>
    struct predicate : public std::is_same<T, U>
    {
    };
};
static_assert(L2::any<same_as_predicate<long>::predicate>);
static_assert(not L2::any<same_as_predicate<int>::predicate>);
static_assert(L2::any<std::is_floating_point>);
static_assert(not L0::any<std::is_floating_point>);
