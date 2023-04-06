#pragma once

#include <type_traits>

namespace bits_of_q {
/// IS EMPTY
template <typename LIST>
struct is_empty : std::false_type
{
};

template <template <typename...> class LIST>
struct is_empty<LIST<>> : std::true_type
{
};

template <typename LIST>
inline constexpr auto is_empty_v = is_empty<LIST>::value;

/// MAKE EMPTY
template <typename LIST>
struct make_empty;

template <template <typename...> class LIST, typename... T0toN>
struct make_empty<LIST<T0toN...>> : public std::type_identity<LIST<>>
{
};

template <typename LIST>
using make_empty_t = typename make_empty<LIST>::type;

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

/// BACK
template <typename LIST>
struct back;

template <template <typename...> class LIST, typename T0, typename... T1toN>
struct back<LIST<T0, T1toN...>> : public back<LIST<T1toN...>>
{
};

template <template <typename...> class LIST, typename T0>
struct back<LIST<T0>> : public std::type_identity<T0>
{
};

template <typename LIST>
using back_t = typename back<LIST>::type;

/// PUSH_BACK
template <typename LIST, typename T>
struct push_back;

template <template <typename...> class LIST, typename... T0toN, typename T1>
struct push_back<LIST<T0toN...>, T1> : public std::type_identity<LIST<T0toN..., T1>>
{
};

template <typename LIST, typename T>
using push_back_t = typename push_back<LIST, T>::type;

/// PUSH_FRONT
template <typename LIST, typename T>
struct push_front;

template <template <typename...> class LIST, typename... T0toN, typename T>
struct push_front<LIST<T0toN...>, T> : public std::type_identity<LIST<T, T0toN...>>
{
};

template <typename LIST, typename T>
using push_front_t = typename push_front<LIST, T>::type;

/// POP_BACK
template <typename LIST, typename RET_LIST = make_empty_t<LIST>>
struct pop_back;

template <template <typename...> class LIST, typename T0, typename RET_LIST>
struct pop_back<LIST<T0>, RET_LIST> : public std::type_identity<RET_LIST>
{
};

template <template <typename...> class LIST, typename RET_LIST>
struct pop_back<LIST<>, RET_LIST> : public std::type_identity<RET_LIST>
{
};

template <template <typename...> class LIST, typename T0, typename T1, typename... T2toN,
          typename RET_LIST>
struct pop_back<LIST<T0, T1, T2toN...>, RET_LIST>
    : public pop_back<LIST<T1, T2toN...>, push_back_t<RET_LIST, T0>>
{
};

template <typename LIST>
using pop_back_t = typename pop_back<LIST>::type;

/// AT
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

/// ANY
template <template <typename> class PREDICATE, typename LIST>
struct any : public std::conditional_t<PREDICATE<front_t<LIST>>::value, std::true_type,
                                       any<PREDICATE, pop_front_t<LIST>>>
{
};

template <template <typename> class PREDICATE, template <typename...> class LIST>
struct any<PREDICATE, LIST<>> : std::false_type
{
};

template <template <typename> class PREDICATE, typename LIST>
inline constexpr bool any_v = any<PREDICATE, LIST>::value;

/// CONTAINS
template <typename T>
struct same_as
{
    template <typename U>
    struct predicate : public std::is_same<T, U>
    {
    };
};

template <typename SEARCH, typename LIST>
struct contains : public any<same_as<SEARCH>::template predicate, LIST>
{
};

template <typename SEARCH, typename LIST>
inline constexpr bool contains_v = contains<SEARCH, LIST>::value;

/// STATIC_FOR
template <int FIRST, int LAST, typename LAMBDA>
constexpr void static_for(const LAMBDA& f)
{
    if constexpr (FIRST < LAST) {
        f(std::integral_constant<int, FIRST>{});
        static_for<FIRST + 1, LAST>(f);
    }
}

/// NOT
template <template <typename...> class PRED>
struct not_
{
    template <typename... Ts>
    using type = std::integral_constant<bool, !PRED<Ts...>::value>;
};
} // namespace bits_of_q
