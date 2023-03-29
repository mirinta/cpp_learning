#pragma once

#include <concepts>

struct NormalType
{
    ~NormalType() = default; /// default destructable
};
static_assert(std::is_trivially_destructible_v<NormalType>);

struct SpecialType
{
    ~SpecialType() { release(); }; /// not default destructable

    void release(); /// a special mechanism to release resources
};
static_assert(not std::is_trivially_destructible_v<SpecialType>);

/// the Wrapper's destructor is the same as the wrapped type T
template <typename T>
struct Wrapper
{
    ~Wrapper() requires requires(T t) { t.release(); }
    {
        data.release();
    }

    ~Wrapper() = default;

private:
    T data;
};
static_assert(not std::is_trivially_destructible_v<Wrapper<SpecialType>>);
static_assert(std::is_trivially_destructible_v<Wrapper<NormalType>>);
