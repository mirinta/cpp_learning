#pragma once

#include "trait_type_identity.h"

/**
 * If T is a reference type, return the type referred to by T.
 * Otherwise, the type is T.
 */
template <typename T>
struct remove_reference : public type_identity<T>
{
};

template <typename T>
struct remove_reference<T&> : public type_identity<T>
{
};

template <typename T>
struct remove_reference<T&&> : public type_identity<T>
{
};

// the _t helper
template <typename T>
using remove_reference_t = remove_reference<T>::type;
