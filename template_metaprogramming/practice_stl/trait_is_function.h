#pragma once

#include "trait_bool_constant.h"

/**
 * NOTE:
 * The following types are not counted as function types
 * 1. std::function;
 * 2. lambdas;
 * 3. classes with overloaded operator();
 * 4. pointers to functions.
 */
template <typename>
struct is_function : public false_type
{
};

/// partial specialization for regular functions, e.g., bool f(int)
template <typename Ret, typename... Args>
struct is_function<Ret(Args...)> : public true_type
{
};

/// partial specialization for variadic functions, e.g., bool f(int,...)
template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...)> : public true_type
{
};

/// partial specializations for function types that have cv-qualifiers
/// #specializations = 2 (all the above) * 3 (const, volatile, const volatile) = 6
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile> : public true_type
{
};

/// partial specializations for function types that have ref-qualifiers
/// #specializations = 8 (all the above) * 2 (& or &&) = 16
template <typename Ret, typename... Args>
struct is_function<Ret(Args...)&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) &&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const&&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile&&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile&&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...)&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) &&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const&&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile&&> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile&&> : public true_type
{
};

/// partial specializations for noexcept version of the above
/// #specializations = 8 + 16 = 24
template <typename Ret, typename... Args>
struct is_function<Ret(Args...) noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...)& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...)&& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const&& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) volatile&& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args...) const volatile&& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...)& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...)&& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const&& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) volatile&& noexcept> : public true_type
{
};

template <typename Ret, typename... Args>
struct is_function<Ret(Args..., ...) const volatile&& noexcept> : public true_type
{
};

// the _v helper
template <typename T>
inline constexpr auto is_function_v = is_function<T>::value;
