#pragma once

#include <concepts>
#include <memory>
#include <variant>

template <typename T>
concept IsInterface = requires(T t)
{
    {
        t.func()
    }
    ->std::same_as<int>;
};

template <typename T, typename... Ts>
concept SameAsAny = (... or std::same_as<T, Ts>);

template <IsInterface... Ts>
struct OneTypeWrapper
{
    explicit OneTypeWrapper(SameAsAny<Ts...> auto t) : _t(t) {}

    void set_t(SameAsAny<Ts...> auto t) { _t = t; }

private:
    std::variant<Ts...> _t;
};

inline void usage()
{
    struct T1
    {
        int func();
    };
    struct T2
    {
        int func();
    };
    struct T3
    {
        int func();
    };
    struct T4
    {
    };

    using WrapT1orT2 = OneTypeWrapper<T1, T2>;
    // using WrapT1orT4 = Wrapper<T1, T4>; // error, T4 is not a required interface

    WrapT1orT2 wrapper(T1{}); // ok, T1 is in the list, the wrapper owns an object of T1
    // WrapT1orT2 wrapper(T3{}); // error, T3 is not in the list

    wrapper.set_t(T2{}); // ok, T2 is in the list, the wrapper owns an object of T2
    // wrapper.set_t(T3{}); // error, T3 is not in the list
}
