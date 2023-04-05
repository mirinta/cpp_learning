#pragma once

#include "trait_remove_reference.h"
#include "trait_type_identity.h"

namespace detail {
template <typename T>
auto add_pointer_impl(int) -> type_identity<remove_reference_t<T>*>;

template <typename T>
auto add_pointer_impl(...) -> type_identity<T>;
} // namespace detail

template <typename T>
struct add_pointer : public decltype(detail::add_pointer_impl<T>(0))
{
};

template <class T>
using add_pointer_t = typename add_pointer<T>::type;
