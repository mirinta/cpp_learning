#pragma once

template <typename T>
struct type_identity
{
    using type = T;
};

// the _t helper
template <typename T>
using type_identity_t = typename type_identity<T>::type;
