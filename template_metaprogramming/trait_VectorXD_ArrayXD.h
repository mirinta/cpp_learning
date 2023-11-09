#include <array>
#include <concepts>
#include <type_traits>
#include <vector>

namespace internal {
template <typename T, size_t D>
    requires(D > 0)
struct VectorXD : std::type_identity<std::vector<typename VectorXD<T, D - 1>::type>>
{
};

template <typename T>
struct VectorXD<T, 1> : std::type_identity<std::vector<T>>
{
};

template <typename T, size_t... Ds>
    requires(sizeof...(Ds) > 0 && ((Ds > 0) && ...))
struct ArrayXD;

template <typename T, size_t D0, size_t... D1toN>
struct ArrayXD<T, D0, D1toN...>
    : std::type_identity<std::array<typename ArrayXD<T, D1toN...>::type, D0>>
{
};

template <typename T, size_t D0>
struct ArrayXD<T, D0> : std::type_identity<std::array<T, D0>>
{
};
} // namespace internal

template <typename T, size_t D>
using VectorXD = internal::VectorXD<T, D>::type;

template <typename T, size_t... Ds>
using ArrayXD = internal::ArrayXD<T, Ds...>::type;

void example()
{
    // 3D vector
    std::vector<std::vector<std::vector<int>>> v0;
    VectorXD<int, 3> v1;
    static_assert(std::is_same_v<decltype(v0), decltype(v1)>);
    // 3D array, shape = 3x4x5
    std::array<std::array<std::array<int, 5>, 4>, 3> a0;
    ArrayXD<int, 3, 4, 5> a1;
    static_assert(std::is_same_v<decltype(a0), decltype(a1)>);
}
