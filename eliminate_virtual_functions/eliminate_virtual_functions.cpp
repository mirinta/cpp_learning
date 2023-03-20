#pragma once
#include <iostream>
#include <type_traits>
#include <memory>

namespace with_virtual {
struct Interface
{
    virtual ~Interface() = default; /// should consider the Rule of 5
    virtual int func() const = 0;
};

struct MyClass : public Interface
{
    int func() const override { return 999; }
};

void usage(const Interface* const t) { std::cout << t->func() << '\n'; }

} // namespace with_virtual

namespace without_virtual {
template <typename T>
concept HasFunc = requires(T foo)
{
    {
        foo.func()
    }
    ->std::integral;
};

struct MyClass
{
    int func() const { return 666; }
};

// struct OtherClass
//{
//    double func() { return 3.14; }
//};
// static_assert(HasFunc<OtherClass>);

void usage(const HasFunc auto& t) { std::cout << t.func() << '\n'; }

} // namespace without_virtual
