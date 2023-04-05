#pragma once

#include "trait_add_pointer.h"
#include "trait_conditional.h"
#include "trait_is_array.h"
#include "trait_is_function.h"
#include "trait_remove_cv.h"
#include "trait_remove_extent.h"
#include "trait_remove_reference.h"

/**
 * If T names the type "array of U" or "reference to array of U", returns U*.
 * If T is a function type F or a reference thereto, returns add_pointer<F>::type.
 * Otherwise, returns remove_cv<remove_reference<T>>::type.
 */
template <typename T>
struct decay
{
private:
    using U = remove_reference_t<T>;

public:
    using type = conditional_t<is_array_v<U>, add_pointer_t<remove_extent_t<U>>,
                               conditional_t<is_function_v<U>, add_pointer_t<U>, remove_cv_t<U>>>;
};

// the _t helper
template <typename T>
using decay_t = typename decay<T>::type;
