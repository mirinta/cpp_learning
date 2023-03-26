#pragma once

#include <concepts>
#include <tuple>
#include <vector>

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
struct MultiTypesWrapper
{
public:
    template <SameAsAny<Ts...> T>
    void add(T&& t)
    {
        std::get<std::vector<T>>(data).push_back(std::forward<T>(t));
    }

private:
    std::tuple<std::vector<Ts>...> data;
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

    using StoreT1AndT2 = MultiTypesWrapper<T1, T2>;
    StoreT1AndT2 store{};
    store.add(T1{});
    store.add(T2{});
}
