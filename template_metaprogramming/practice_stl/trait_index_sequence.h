#pragma once

#include "trait_type_identity.h"

/**
 * Reference:
 * https://stackoverflow.com/questions/17424477/implementation-c14-make-integer-sequence
 */
template <size_t... Is>
struct index_sequence
{
    static constexpr size_t size() noexcept { return sizeof...(Is); }
};

namespace detail {
template <typename SEQ1, typename SEQ2>
struct merge;

template <size_t... I1s, size_t... I2s>
struct merge<index_sequence<I1s...>, index_sequence<I2s...>>
    : public type_identity<index_sequence<I1s..., (sizeof...(I1s) + I2s)...>>
{
};

template <typename SEQ1, typename SEQ2>
using merge_t = typename merge<SEQ1, SEQ2>::type;

template <size_t SIZE>
struct make_sequence : public type_identity<merge_t<typename make_sequence<SIZE / 2>::type,
                                                    typename make_sequence<SIZE - SIZE / 2>::type>>
{
};

template <>
struct make_sequence<1> : public type_identity<index_sequence<0>>
{
};

template <>
struct make_sequence<0> : public type_identity<index_sequence<>>
{
};
} // namespace detail

template <size_t SIZE>
using make_index_sequence = typename detail::make_sequence<SIZE>::type;
