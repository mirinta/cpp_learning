#pragma once

template <typename T, T t>
struct integral_constant
{
    using value_type = T;
    using type = integral_constant;
    static constexpr T value = t;

    // operator()
    constexpr value_type operator()() const noexcept { return t; }

    // conversion function
    constexpr operator value_type() const noexcept { return t; }
};
