#pragma once

#include <functional>

#include "traits.h"

namespace mrnt {
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

/// Helper function to get the number of types in a tuple
template <typename TUPLE>
struct tuple_size;

template <typename... ELEMS>
struct tuple_size<Tuple<ELEMS...>> : std::integral_constant<size_t, sizeof...(ELEMS)>
{
};

template <typename TUPLE>
inline constexpr auto tuple_size_v = tuple_size<TUPLE>::value;

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

/// Helper function: to construct a tuple of references to the arguments in args suitable for
/// forwarding as an argument to a function. The tuple has rvalue reference data members when
/// rvalues are used as arguments, and otherwise has lvalue reference data members.
/// @see std::forward_as_tuple
template <typename... Ts>
constexpr auto forward_as_tuple(Ts&&... args) noexcept
{
    return Tuple<Ts&&...>(std::forward<Ts>(args)...);
}

namespace detail {
template <typename FWD_INDEX_SEQ, typename TUPLE_INDEX_SEQ>
struct concat_with_fwd_tuple;

template <size_t... FWD_Is, size_t... TUPLE_Is>
struct concat_with_fwd_tuple<std::index_sequence<FWD_Is...>, std::index_sequence<TUPLE_Is...>>
{
    template <typename FWD_TUPLE, typename TUPLE>
    static constexpr auto f(FWD_TUPLE&& fwd, TUPLE&& tuple)
    {
        return mrnt::forward_as_tuple(get<FWD_Is>(std::forward<FWD_TUPLE>(fwd))...,
                                      get<TUPLE_Is>(std::forward<TUPLE>(tuple))...);
    }
};

template <typename INDEX_SEQ>
struct make_tuple_from_fwd_tuple;

template <size_t... Is>
struct make_tuple_from_fwd_tuple<std::index_sequence<Is...>>
{
    template <typename FWD_TUPLE>
    static constexpr auto f(FWD_TUPLE&& fwd)
    {
        return Tuple{get<Is>(std::forward<FWD_TUPLE>(fwd))...};
    }
};

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
        return f(concat_with_fwd_tuple<
                     std::make_index_sequence<tuple_size_v<std::remove_cvref_t<FWD_TUPLE>>>,
                     std::make_index_sequence<tuple_size_v<std::remove_cvref_t<TUPLE>>>>::
                     f(std::forward<FWD_TUPLE>(fwd), std::forward<TUPLE>(t)),
                 std::forward<TUPLES>(tuples)...);
    }

    template <typename FWD_TUPLE>
    static constexpr auto f(FWD_TUPLE&& fwd)
    {
        return make_tuple_from_fwd_tuple<std::make_index_sequence<
            tuple_size_v<std::remove_cvref_t<FWD_TUPLE>>>>::f(std::forward<FWD_TUPLE>(fwd));
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
    return detail::tuple_cat_impl::f(std::forward<TUPLES>(tuples)...);
}
} // namespace mrnt
