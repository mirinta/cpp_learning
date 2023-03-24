#pragma once

#include <concepts>

template <typename T>
concept IsInterface = requires(T t)
{
    {
        t.func()
    }
    ->std::same_as<int>;
};

void usage(IsInterface auto&& t);

template <bool hasFunc, bool validFunc>
struct TestClass
{
    int func() requires(hasFunc&& validFunc);
    void func(int) requires(hasFunc && not validFunc);
};

using NoInterface = TestClass<false, true>;
static_assert(not IsInterface<NoInterface>);

using InvalidInterface = TestClass<true, false>;
static_assert(not IsInterface<InvalidInterface>);

using ValidInterface = TestClass<true, true>;
static_assert(IsInterface<ValidInterface>);
