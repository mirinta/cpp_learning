namespace internal {
template <typename T, size_t D>
struct Matrix
{
    using type = std::vector<typename Matrix<T, D - 1>::type>;
};

template <typename T>
struct Matrix<T, 0>
{
    using type = T;
};
} // namespace internal

template <typename T>
using Matrix2D = internal::Matrix<T, 2>::type;
static_assert(std::is_same_v<Matrix2D<int>, std::vector<std::vector<int>>>);

template <typename T>
using Matrix3D = internal::Matrix<T, 3>::type;
static_assert(std::is_same_v<Matrix3D<int>, std::vector<Matrix2D<int>>>);

template <typename T>
using Matrix4D = internal::Matrix<T, 4>::type;
static_assert(std::is_same_v<Matrix4D<int>, std::vector<Matrix3D<int>>>);
