#pragma once

#include <type_traits>
#include <vector>
#include <list>
#include <iostream>

namespace my_traits {
/// integral constant
template <typename T, T val>
struct integral_constant
{
    static constexpr T value = val;
    using value_type = T;
    using type = integral_constant<T, val>;

    /// the conversion function
    constexpr operator value_type() noexcept { return val; }

    /// the call operator
    constexpr value_type operator()() noexcept { return val; }
};
static_assert(integral_constant<int, 5u>::value == 5u);
static_assert(!integral_constant<bool, false>::value);

/// bool constant
template <bool val>
using bool_constant = integral_constant<bool, val>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

/// is same (exact the same)
template <typename T1, typename T2>
struct is_same : false_type
{
};

template <typename T1, typename T2>
inline constexpr bool is_same_v = is_same<T1, T2>::value;

template <typename T>
struct is_same<T, T> : true_type
{
};

static_assert(is_same_v<int, int>);
static_assert(not is_same_v<int, const int>);

/// identity type
template <typename T>
struct identity_type
{
    using type = T;
};

/// remove top-level cv-qualifier
template <typename T>
struct remove_cv : public identity_type<T>
{
};

template <typename T>
/// ::type is a dependent name and it is a type
/// so we must use "typename", otherwise the compiler won't treat it as a type
using remove_cv_t = typename remove_cv<T>::type;

template <typename T>
struct remove_cv<const T> : public identity_type<T>
{
};

template <typename T>
struct remove_cv<volatile T> : public identity_type<T>
{
};

template <typename T>
struct remove_cv<const volatile T> : public identity_type<T>
{
};

static_assert(is_same_v<int, remove_cv_t<const int>>);
static_assert(is_same_v<int, remove_cv_t<volatile int>>);
static_assert(is_same_v<int, remove_cv_t<const volatile int>>);

/// is same (cv-qualifiers not taken into account)
template <typename T1, typename T2>
using is_same_raw = is_same<remove_cv_t<T1>, remove_cv_t<T2>>;

template <typename T1, typename T2>
inline constexpr bool is_same_raw_v = is_same_raw<T1, T2>::value;

static_assert(is_same_raw_v<int, const int>);
static_assert(is_same_raw_v<int, volatile int>);
static_assert(is_same_raw_v<int, const volatile int>);

/// is void (consider the cv-qualifiers)
template <typename T>
using is_void = is_same<void, remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

static_assert(is_void_v<void>);
static_assert(is_void_v<volatile void>);
static_assert(is_void_v<const volatile void>);

/// conditional (use T1 if true, otherwise use T2)
template <typename T1, typename T2, bool val>
struct conditional : identity_type<T1>
{
};

template <typename T1, typename T2, bool val>
using conditional_t = typename conditional<T1, T2, val>::type;

template <typename T1, typename T2>
struct conditional<T1, T2, false> : identity_type<T2>
{
};

static_assert(is_same_v<int, conditional_t<int, double, true>>);
static_assert(is_same_v<double, conditional_t<int, double, false>>);

/// is array (with or without specified with a size, and nested arrays)
template <typename T>
struct is_array : public false_type
{
};

template <typename T, size_t N>
struct is_array<T[N]> : public true_type
{
};

template <typename T>
struct is_array<T[]> : public true_type
{
};

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

static_assert(not is_array_v<int>);
static_assert(is_array_v<int[]>);
static_assert(is_array_v<int[1]>);
static_assert(is_array_v<int[2][3]>);
static_assert(is_array_v<int[][4]>);

/// is pointer (with or without low-level cv-qualifiers)
template <typename T>
struct is_pointer : false_type
{
};

/// this partial specialization handle the cases with/without top-level cv-qualifiers
template <typename T>
struct is_pointer<T*> : true_type
{
};

template <typename T>
struct is_pointer<T* const> : true_type
{
};

template <typename T>
struct is_pointer<T* volatile> : true_type
{
};

template <typename T>
struct is_pointer<T* const volatile> : true_type
{
};

template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

static_assert(not is_pointer_v<int>);
static_assert(is_pointer_v<int*>);
static_assert(is_pointer_v<int* const>);
static_assert(is_pointer_v<int* volatile>);
static_assert(is_pointer_v<int* const volatile>);
static_assert(is_pointer_v<const int*>);
static_assert(is_pointer_v<volatile int*>);
static_assert(is_pointer_v<const volatile int*>);

/// is null pointer
/// the literal "nullptr" is a value, not a type
using nullptr_t = decltype(nullptr);

template <typename T>
using is_nullptr = is_same<nullptr_t, remove_cv_t<T>>;

template <typename T>
inline constexpr bool is_nullptr_t = is_nullptr<T>::value;

static_assert(not is_nullptr_t<int>);
static_assert(is_nullptr_t<nullptr_t>);
static_assert(is_nullptr_t<const nullptr_t>);
static_assert(is_nullptr_t<volatile nullptr_t>);
static_assert(is_nullptr_t<const volatile nullptr_t>);

/// void_t
template <typename...>
using void_t = void;

static_assert(is_same_v<void, void_t<>>);
static_assert(is_same_v<void, void_t<int, double, std::string>>);

/// is class or union
// version 1:
// template <typename T>
// true_type is_class_or_union_impl(int T::*);

// template <typename T>
// false_type is_class_or_union_impl(...);

// template <typename T>
// using is_class_or_union = decltype(is_class_or_union_impl<T>(nullptr));
// version 2:
template <typename T, typename = void>
struct is_class_or_union : public std::false_type
{
};

template <typename T>
struct is_class_or_union<T, std::void_t<int T::*>> : public std::true_type
{
};

template <typename T>
inline constexpr bool is_class_or_union_v = is_class_or_union<T>::value;

static_assert(not is_class_or_union_v<int>);
static_assert(not is_class_or_union_v<int[]>);
static_assert(not is_class_or_union_v<char*>);
class C
{
};
union U {
};
static_assert(is_class_or_union_v<C>);
static_assert(is_class_or_union_v<U>);

/// is class
template <typename T>
inline constexpr bool is_class_v = is_class_or_union_v<T> && !std::is_union_v<T>;

template <typename T>
using is_class = bool_constant<is_class_v<T>>;

static_assert(is_class_v<C>);
static_assert(is_class_v<const C>);
static_assert(not is_class_v<U>);
static_assert(not is_class_v<int>);

/// is one of (exactly matched in the given list)
/// T is in the given type list {P0, ..., PN}
template <typename T, typename... P0toN>
struct is_one_of;

/// partial specialization:
/// the list is empty
template <typename T>
struct is_one_of<T> : public false_type
{
};

/// partial specialization:
/// T is at the fisrt of the list, i.e., {T, P1, ..., PN}
template <typename T, typename... P1toN>
struct is_one_of<T, T, P1toN...> : true_type
{
};

/// partial specialization:
/// T is not at the first of the list, i.e., {P0, P2, ..., PN}
/// recursively use the primary template
template <typename T, typename P0, typename... P1toN>
struct is_one_of<T, P0, P1toN...> : is_one_of<T, P1toN...>
{
};

template <typename T, typename... P0toN>
inline constexpr bool is_one_of_v = is_one_of<T, P0toN...>::value;

static_assert(not is_one_of_v<int>);
static_assert(not is_one_of_v<int, double, char, int*, const int>);
static_assert(is_one_of_v<int, double, char, int>);

/// is_container, a naive implementation
/// every STL container has a pointer to iterator
// version 1:
// template <typename T>
// true_type is_stl_container_impl(typename T::iterator*);

// template <typename T>
// false_type is_stl_container_impl(...);

// template <typename T>
// using is_stl_container = decltype(is_stl_container_impl<T>(nullptr));

// version 2:
// template <typename T>
// struct is_stl_container
//{
//    template <typename U>
//    static int test(typename U::iterator*);

//    template <typename U>
//    static char test(...);

//    static constexpr bool value = sizeof(int) == sizeof(test<T>(nullptr));
//};

// version 3:
template <typename T, typename = void>
struct is_stl_container : public std::false_type
{
};

template <typename T>
struct is_stl_container<T, std::void_t<typename T::iterator>> : public std::true_type
{
};

template <typename T>
inline constexpr bool is_stl_container_v = is_stl_container<T>::value;

static_assert(not is_stl_container_v<int>);
static_assert(is_stl_container_v<std::vector<int>>);
static_assert(is_stl_container_v<const std::vector<int>>);
static_assert(is_stl_container_v<volatile std::vector<int>>);
static_assert(is_stl_container_v<const volatile std::vector<int>>);
static_assert(is_stl_container_v<std::list<std::vector<int>>>);

/// are all integral, notice std::conjunction<>::value = true
// version 1: empty list is allowed but it returns false
template <typename... Ts>
struct are_all_integral : public std::conjunction<std::is_integral<Ts>...>
{
};

template <>
struct are_all_integral<> : public std::false_type
{
};

template <typename... Ts>
inline constexpr bool are_all_integral_v = are_all_integral<Ts...>::value;

// template <typename... Ts>
// inline constexpr bool are_all_integral_v = are_all_integral<Ts...>::value;
// version 2: emplty list is not allowed
// template <typename T, typename... Ts>
// struct are_all_integral : public std::conjunction<std::is_integral<T>, std::is_integral<Ts>...>
//{
//};

// template <typename T, typename... Ts>
// inline constexpr bool are_all_integral_v = are_all_integral<T, Ts...>::value;

static_assert(not are_all_integral_v<>);
static_assert(not are_all_integral_v<double, int>);
static_assert(are_all_integral_v<size_t, int>);
} // namespace my_traits
