#pragma once

#include <functional>

#include "traits.h"

namespace bits_of_q {
template <typename... ELEMS>
struct Tuple
{
    /// Constructor: empty tuple, no "data" member
    constexpr Tuple() = default;
};

/// Recursive inheritance
/// Example: Tuple<E0, E1, E2> {E0 data;} -> Tuple<E1, E2> {E1 data;} -> Tuple<E2> {E2 data;}
template <typename E0, typename... E1toN>
struct Tuple<E0, E1toN...> : public Tuple<E1toN...>
{
    /// Constructor: multiple types, only owns the data of the first type (E0)
    /// ********** IMPORTANT **********
    /// 1. Without forwarding reference, if the data type is Ei,
    /// then #unnecessary_copy_data += i, due to the recursive inheritance.
    ///
    /// 2. Ej&& is not a forwarding reference!
    /// We need new template parameters for the constructor
    template <typename T0, typename... T1toN>
    explicit constexpr Tuple(T0&& e0, T1toN&&... e1toN)
        : Tuple<E1toN...>(std::forward<T1toN>(e1toN)...), data(std::forward<T0>(e0))
    {
    }
    E0 data; /// here, copy e0 to "data" member is acceptable
};

/// Deduction guide to make template argument dedcution for constructors work (C++17)
/// TODO: deduction guide?
/// Example: Tuple tup(1, true, 3.14)
template <typename E0, typename... E1toN>
Tuple(E0, E1toN...) -> Tuple<std::unwrap_ref_decay_t<E0>, std::unwrap_ref_decay_t<E1toN>...>;

/// Helper function to make a tuple, @see std::make_tuple
/// TODO: std::unwrap_ref_decay_t?
/// ********** IMPORTANT **********
/// without forwarding reference, #unnecessary_copy_data += 1
template <typename... ELEMS>
auto make_tuple(ELEMS&&... elems)
{
    return Tuple<std::unwrap_ref_decay_t<ELEMS>...>{std::forward<ELEMS>(elems)...};
};

////////////////////////////////////////////////////////////
///////////////////////  tuple_size  ///////////////////////
////////////////////////////////////////////////////////////

template <typename TUPLE>
struct tuple_size;

template <typename... ELEMS>
struct tuple_size<Tuple<ELEMS...>> : std::integral_constant<size_t, sizeof...(ELEMS)>
{
};

template <typename TUPLE>
inline constexpr auto tuple_size_v = tuple_size<TUPLE>::value;

////////////////////////////////////////////////////////////
///////////////////////////  get  //////////////////////////
////////////////////////////////////////////////////////////

namespace detail {
template <size_t I, typename TUPLE>
struct get_impl : public get_impl<I - 1, pop_front_t<TUPLE>>
{
};

template <typename TUPLE>
struct get_impl<0, TUPLE>
{
    /// After the type list is manipulated to the expected form,
    /// We need to extract the data from it
    /// TODO: remove_ref before using is_const?
    template <typename T>
    static constexpr decltype(auto) get(T&& t)
    {
        /// ********** IMPORTANT **********
        /// 1. The real input tuple is perfect forwarded to here,
        /// which means T may have ref or cv-qualifers.
        /// To access the "data" member, we need to cast it to TUPLE.
        /// But remember the TUPLE's ref and cv-qualifiers are removed?
        /// Thus, we need to restore its ref and cv-qualifiers.
        ///
        /// 2. If we directly return static_cast<...>(t).data, it is just a value type Et,
        /// i.e., assignment operations like get<...>(tuple) = xxx are not allowed.
        /// Thus, we need to ensure the return type has the same ref and cv-qualifiers as the input
        /// tuple.
        /// To do so, we need to obtain the the front type in the type list.
        /// Then apply the same ref and cv-qualifiers as the input tuple.
        constexpr auto is_lvalue = std::is_lvalue_reference_v<T>;
        constexpr auto is_const = std::is_const_v<std::remove_reference_t<T>>;
        using data_t = front_t<TUPLE>;

        if constexpr (is_lvalue && is_const) {
            return static_cast<const data_t&>(static_cast<const TUPLE&>(t).data);
        }
        if constexpr (is_lvalue && !is_const) {
            return static_cast<data_t&>(static_cast<TUPLE&>(t).data);
        }
        if constexpr (!is_lvalue && is_const) {
            return static_cast<const data_t&&>(static_cast<const TUPLE&&>(t).data);
        }
        if constexpr (!is_lvalue && !is_const) {
            return static_cast<data_t&&>(static_cast<TUPLE&&>(t).data);
        }
    }
};
} // namespace detail

/// Helper function to get the data of the Ith type, @see std::get
/// TODO: decltype(auto)? deduce reference types?
template <size_t I, typename TUPLE>
constexpr decltype(auto) get(TUPLE&& tuple)
{
    /// ********** IMPORTANT **********
    /// TUPLE is a forwarding reference, it may have ref and cv-qualifiers.
    /// We need to remove them for using the type traits.
    return detail::get_impl<I, std::remove_cvref_t<TUPLE>>::get(std::forward<TUPLE>(tuple));
}

////////////////////////////////////////////////////////////
////////////////////  forward_as_tuple  ////////////////////
////////////////////////////////////////////////////////////

template <typename... Ts>
constexpr auto forward_as_tuple(Ts&&... args) noexcept
{
    return Tuple<Ts&&...>{std::forward<Ts>(args)...};
}

////////////////////////////////////////////////////////////
///////////////////////  tuple_cat  ////////////////////////
////////////////////////////////////////////////////////////

namespace detail {
template <typename... TUPLES>
struct tuple_cat_result;

template <template <typename...> class TUPLE, typename... T1s, typename... T2s, typename... TUPLES>
struct tuple_cat_result<TUPLE<T1s...>, TUPLE<T2s...>, TUPLES...>
    : public tuple_cat_result<TUPLE<T1s..., T2s...>, TUPLES...>
{
};

template <typename TUPLE>
struct tuple_cat_result<TUPLE> : public std::type_identity<TUPLE>
{
};

template <typename... TUPLES>
using tuple_cat_result_t = typename tuple_cat_result<TUPLES...>::type;

template <typename RESULT_TUPLE, typename INDEX_SEQ>
struct make_tuple_from_forward_tuple;

template <typename RESULT_TUPLE, size_t... Is>
struct make_tuple_from_forward_tuple<RESULT_TUPLE, std::index_sequence<Is...>>
{
    template <typename FWD_TUPLE>
    static constexpr auto f(FWD_TUPLE&& fwd)
    {
        return RESULT_TUPLE{get<Is>(std::forward<FWD_TUPLE>(fwd))...};
    }
};

template <typename FWD_INDEX_SEQ, typename TUPLE_INDEX_SEQ>
struct concat_with_forward_tuple;

template <size_t... FWD_Is, size_t... TUPLE_Is>
struct concat_with_forward_tuple<std::index_sequence<FWD_Is...>, std::index_sequence<TUPLE_Is...>>
{
    template <typename FWD_TUPLE, typename TUPLE>
    static constexpr auto f(FWD_TUPLE&& fwd, TUPLE&& tuple)
    {
        return forward_as_tuple(get<FWD_Is>(std::forward<FWD_TUPLE>(fwd))...,
                                get<TUPLE_Is>(std::forward<TUPLE>(tuple))...);
    }
};

template <typename RESULT_TUPLE>
struct tuple_cat_impl
{
    /// version 1: concate only two tuples using two corresponding index sequences
    /// e.g., t1 has N elements, t2 has M elements, then
    /// the result is Tuple(get<0>(t1),...,get<N-1>(t1),get<0>(t2),...,get<M-1>(t2))
    /// ********** IMPORTANT **********
    /// Calling std::forward multiple times on a same input is dangerous!
    /// If the input is a lvalue reference, then std::forward = std::move,
    /// which means the input is going to be destroyed after the first time of calling
    /// std::forward.
    //    template <typename TUPLE1, typename TUPLE2>
    //    static auto f(TUPLE1&& tuple1, TUPLE2&& tuple2)
    //    {
    //        return cat_from_indices(
    //            std::forward<TUPLE1>(tuple1), std::forward<TUPLE2>(tuple2),
    //            std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE1>>>(),
    //            std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE2>>>());
    //    }
    /// version 2: concate multiple tuples using recursion
    /// ********** IMPORTANT **********
    /// the RET_TUPLE is set at the beginning,
    /// so it will be moved a lot of times during the recursion.
    //    template <typename RET_TUPLE, typename TUPLE, typename... TUPLES>
    //    static constexpr auto f(RET_TUPLE&& ret, TUPLE&& t, TUPLES&&... tuples)
    //    {
    //        return f(cat_from_indices(
    //                     std::forward<RET_TUPLE>(ret), std::forward<TUPLE>(t),
    //                     std::make_index_sequence<tuple_size_v<std::remove_cvref_t<RET_TUPLE>>>(),
    //                     std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE>>>()),
    //                 std::forward<TUPLES>(tuples)...);
    //    }

    //    template <typename RET_TUPLE>
    //    static constexpr auto f(RET_TUPLE&& ret)
    //    {
    //        return std::forward<RET_TUPLE>(ret);
    //    }

    //    template <typename TUPLE1, typename TUPLE2, size_t... I1s, size_t... I2s>
    //    static auto cat_from_indices(TUPLE1&& tuple1, TUPLE2&& tuple2,
    //    std::index_sequence<I1s...>,
    //                                 std::index_sequence<I2s...>)
    //    {
    //        return Tuple{get<I1s>(std::forward<TUPLE1>(tuple1))...,
    //                     get<I2s>(std::forward<TUPLE2>(tuple2))...};
    //    }
    /// version 3:
    /// ********** IMPORTANT **********
    /// To eliminate unnecessary move operations:
    /// - recursively build forwarding tuple from inputs;
    /// - once done, convert forwarding tuple to output tuple.
    template <typename FWD_TUPLE, typename TUPLE, typename... TUPLES>
    static constexpr auto f(FWD_TUPLE&& fwd, TUPLE&& t, TUPLES&&... tuples)
    {
        return f(concat_with_forward_tuple<
                     std::make_index_sequence<tuple_size_v<std::remove_cvref_t<FWD_TUPLE>>>,
                     std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE>>>>::
                     f(std::forward<FWD_TUPLE>(fwd), std::forward<TUPLE>(t)),
                 std::forward<TUPLES>(tuples)...);
    }

    /// overload: recursion finished
    template <typename FWD_TUPLE>
    static constexpr auto f(FWD_TUPLE&& fwd)
    {
        return make_tuple_from_forward_tuple<
            RESULT_TUPLE, std::make_index_sequence<tuple_size_v<std::remove_cvref_t<FWD_TUPLE>>>>::
            f(std::forward<FWD_TUPLE>(fwd));
    }
};
} // namespace detail

/// Helper function to concatenate multiple Tuples
/// Example: concate Tuple{"0",1} and Tuple{"2",3.14} should return Tuple{"0",1,"2",3.14}
/// ********** IMPORTANT **********
/// use forwarding reference to avoid unnecessary copies
template <typename... TUPLES>
constexpr auto tuple_cat(TUPLES&&... tuples)
{
    return detail::tuple_cat_impl<detail::tuple_cat_result_t<std::remove_cvref_t<TUPLES>...>>::f(
        std::forward<TUPLES>(tuples)...);
}

////////////////////////////////////////////////////////////
///////////////////////  transform  ////////////////////////
////////////////////////////////////////////////////////////

namespace detail {
template <typename TUPLE, typename FUNC, size_t... Is>
constexpr auto transform_impl(TUPLE&& tuple, const FUNC& f, std::index_sequence<Is...>)
{
    return Tuple{f(get<Is>(std::forward<TUPLE>(tuple)))...};
}
} // namespace detail

template <typename TUPLE, typename FUNC>
constexpr auto transform(TUPLE&& tuple, const FUNC& f)
{
    return detail::transform_impl(
        std::forward<TUPLE>(tuple), f,
        std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE>>>{});
}

////////////////////////////////////////////////////////////
////////////////////  tuple_element_t  /////////////////////
////////////////////////////////////////////////////////////

template <typename TUPLE, size_t I>
using tuple_element_t = at_t<TUPLE, I>;

////////////////////////////////////////////////////////////
/////////////////////////  filter  /////////////////////////
////////////////////////////////////////////////////////////

namespace detail {
template <typename TUPLE, template <typename> class PREDICATE>
struct filter_result;

template <template <typename...> class TUPLE, typename... ELEMS,
          template <typename> class PREDICATE>
struct filter_result<TUPLE<ELEMS...>, PREDICATE>
{
    template <typename ELEM>
    struct wrap_if_pred_matches
    {
        using type = std::conditional_t<PREDICATE<ELEM>::value, TUPLE<ELEM>, TUPLE<>>;
    };
    using type = tuple_cat_result_t<typename wrap_if_pred_matches<ELEMS>::type...>;
};

template <typename TUPLE, template <typename> class PREDICATE>
using filter_result_t = typename filter_result<TUPLE, PREDICATE>::type;

template <typename RESULT_TUPLE, typename WRAP_TUPLE, size_t... Is>
constexpr auto cat_tuple_content_as(WRAP_TUPLE&& wrap, std::index_sequence<Is...>)
{
    return tuple_cat_impl<RESULT_TUPLE>::f(get<Is>(std::forward<WRAP_TUPLE>(wrap))...);
}

template <typename WRAP_TUPLE, size_t... Is>
constexpr auto cat_tuple_content(WRAP_TUPLE&& wrap, std::index_sequence<Is...>)
{
    return tuple_cat(get<Is>(std::forward<WRAP_TUPLE>(wrap))...);
}

template <template <typename> class PREDICATE, typename TUPLE, size_t... Is>
constexpr auto wrap_filtered(TUPLE&& tuple, std::index_sequence<Is...>)
{
    auto wrap_if_pred_matches = [&](auto i) {
        if constexpr (PREDICATE<tuple_element_t<std::remove_cvref_t<TUPLE>, i.value>>::value) {
            /// ********** IMPORTANT **********
            /// intermediate result, it should be a forward tuple
            return forward_as_tuple(get<i.value>(std::forward<TUPLE>(tuple)));
        } else {
            return Tuple<>{};
        }
    };
    return Tuple{wrap_if_pred_matches(std::integral_constant<size_t, Is>{})...};
}
} // namespace detail

template <template <typename> class PREDICATE, typename TUPLE>
constexpr auto filter(TUPLE&& tuple)
{
    /// version 1:
    /// 1. transform each element to a one-element tuple based on the given predicate:
    /// - if true, return Tuple{elem};
    /// - otherwise, return an empty tuple.
    /// 2. concate all intermediate tuples to get the final result.
    /// ********** IMPORTANT **********
    /// The following implementation uses the "transform" function to pass the elements
    /// to a lambda. It has the inherent problem that the lambda is unable to distinguish wheter the
    /// element type is a value or a lvalue reference.
    ///
    /// SOLUTION:
    /// - Add a tuple_element_t using the "at" function.
    /// - Instead of using transform, create a similar function that executes the predicate on the
    /// tuple_element_at at each index instead of using the decltype of the passed element.
    //    auto wrap_if_pred_matches = [&]<typename ELEM>(ELEM&& e) {
    //        if constexpr (PREDICATE<std::remove_cvref_t<ELEM>>::value) {
    //            return forward_as_tuple(std::forward<ELEM>(e));
    //        } else {
    //            return Tuple<>{};
    //        }
    //    };
    //    auto wrapped_tuple = transform(std::forward<TUPLE>(tuple), wrap_if_pred_matches);
    /// version 2:
    auto wrapped_tuple = detail::wrap_filtered<PREDICATE>(
        std::forward<TUPLE>(tuple),
        std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE>>>{});
    return detail::cat_tuple_content_as<
        detail::filter_result_t<std::remove_cvref_t<TUPLE>, PREDICATE>>(
        std::move(wrapped_tuple),
        std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE>>>{});
}
} // namespace bits_of_q
