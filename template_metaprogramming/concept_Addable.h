#pragma once

#include <concepts>

#include "trait_are_same.h"
#include "trait_arguments_count.h"
#include "trait_first_type.h"

/**
 * @brief The arguments are addable if all the following requirements are satisfied:
 * 1. At least two args;
 * 2. All types are exact the same;
 * 3. The operator+ is available;
 * 4. The operator+ is noexcept and the return type is the same as the first arg type.
 */
template <typename... Args>
concept Addable = requires(Args... args)
{
    requires arguments_count_v<Args...> > 1;
    requires are_same_v<Args...>;
    (... + args);
    {
        (... + args)
    }
    noexcept->std::same_as<first_type_t<Args...>>;
};

template <typename... Args>
requires Addable<Args...> auto add(Args&&... args)
{
    return (... + args);
}

namespace test_Addable {
template <bool withNoExcept, bool hasOperatorPlus, bool validReturnType>
struct Test
{
    Test operator+(const Test&) noexcept(withNoExcept) requires(hasOperatorPlus&& validReturnType)
    {
        return *this;
    }

    int operator+(const Test&) noexcept(not withNoExcept) requires(hasOperatorPlus &&
                                                                   not validReturnType)
    {
        return 0;
    }
};

using ValidClass = Test<true, true, true>;
using NoOperatorPlus = Test<true, false, true>;
using WithoutNoExcept = Test<false, true, true>;
using DifferentReturnType = Test<true, true, false>;

static_assert(not Addable<int>);
static_assert(not Addable<int, double>);
static_assert(Addable<int, int>);
static_assert(Addable<ValidClass, ValidClass>);
static_assert(not Addable<NoOperatorPlus, NoOperatorPlus>);
static_assert(not Addable<WithoutNoExcept, WithoutNoExcept>);
static_assert(not Addable<DifferentReturnType, DifferentReturnType>);
} // namespace test_Addable
