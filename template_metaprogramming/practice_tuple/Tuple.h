#pragma once

#include <utility>

#include "traits.h"

template <typename... ELEMS>
struct Tuple
{
    constexpr Tuple() = default;
};

/// Recursive inheritance
/// Example: Tuple<E0, E1, E2> {E0 data;} -> Tuple<E1, E2> {E1 data;} -> Tuple<E2> {E2 data;}
template <typename E0, typename... E1toN>
struct Tuple<E0, E1toN...> : public Tuple<E1toN...>
{
    /// Constructor
    explicit constexpr Tuple(E0 e0, E1toN... e1toN) : Tuple<E1toN...>(e1toN...), data(e0) {}
    E0 data;
};

/// Deduction guide to make template argument dedcution for constructors work (C++17)
/// TODO: deduction guide?
/// Example: Tuple tup(1, true, 3.14)
template <typename E0, typename... E1toN>
Tuple(E0, E1toN...) -> Tuple<std::unwrap_ref_decay_t<E0>, std::unwrap_ref_decay_t<E1toN>...>;

/// Helper function to make a tuple, @see std::make_tuple
/// TODO: std::unwrap_ref_decay_t?
/// version 1: all arguments are passed by value
template <typename... ELEMS>
auto make_tuple(ELEMS... elems)
{
    return Tuple<std::unwrap_ref_decay_t<ELEMS>...>{elems...};
};

namespace detail {
template <size_t I, typename TUPLE>
struct get_impl : public get_impl<I - 1, pop_front_t<TUPLE>>
{
    /// Use pop_front_t to manipulate the type list
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
        /// IMPORTANT:
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
        /// To do so, we need to obtain the Ith type in the type list.
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
    /// IMPORTANT:
    /// TUPLE is a forwarding reference, it may have ref and cv-qualifiers.
    /// We need to remove them for using the type traits.
    return detail::get_impl<I, std::remove_cvref_t<TUPLE>>::get(std::forward<TUPLE>(tuple));
}
