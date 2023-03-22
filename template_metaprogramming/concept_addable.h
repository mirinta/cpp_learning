#pragma once

#include <concepts>

#include "trait_are_same.h"
#include "trait_arguments_count.h"
#include "trait_first_type.h"

template <typename... Args>
concept Addable = requires(Args... args)
{
    requires arguments_count_v<Args...> > 1; // 1. at least two args
    requires are_same_v<Args...>;            // 2. all types are exact the same
    (... + args);                            // 3. operator+ is available
    {
        (... + args)
    }
    noexcept->std::same_as<first_type_t<Args...>>; // 4. the operator+ is noexcept and the return
                                                   // type is the same as the first arg type
};

template <typename... Args>
requires Addable<Args...> auto add(Args&&... args)
{
    return (... + args);
}
