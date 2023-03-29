#pragma once

#include <vector>

template <typename T>
concept Container = requires(T t)
{
    typename T::value_type;
    typename T::size_type;
    typename T::allocator_type;
    typename T::iterator;
    typename T::const_iterator;
    t.size();
    t.begin();
    t.end();
    t.cbegin();
    t.cend();
};

struct A
{
};
static_assert(not Container<A>);
static_assert(Container<std::vector<int>>);
