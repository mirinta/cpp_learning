#include "test_utils_v2.h"
#include "Tuple.h"

constexpr size_t boq_tuple = 1;
constexpr size_t std_tuple = 2;
namespace boq = bits_of_q;
using bits_of_q::testing::Tester;
using bits_of_q::testing::TesterWithBuilder;

int main()
{
    using namespace bits_of_q;

    Tester::test("constructor", []() {
        auto c1 = make_copy_counter<boq_tuple>();
        auto c2 = make_copy_counter<std_tuple>();

        boq::Tuple t1{9, c1, 1.4};
        std::tuple t2{9, c2, 1.4};

        ASSERT_EQ(c1, c2);
    });

    Tester::test("make_tuple", []() {
        auto c1 = make_copy_counter<boq_tuple>();
        auto c2 = make_copy_counter<std_tuple>();

        [[maybe_unused]] auto&& dummy1 = make_tuple(8, c1, 1.1);
        [[maybe_unused]] auto&& dummy2 = std::make_tuple(8, c2, 1.1);

        ASSERT_EQ(c1, c2);
    });

    TesterWithBuilder<1>::test("get", [](auto&& builder) {
        auto c1 = make_copy_counter<boq_tuple>();
        auto c2 = make_copy_counter<std_tuple>();

        auto&& tuple1 = builder.build(boq::Tuple{42, c1, true});
        auto&& tuple2 = builder.build(std::tuple{42, c2, true});

        auto v1 = get<1>(std::forward<decltype(tuple1)>(tuple1));
        auto v2 = get<1>(std::forward<decltype(tuple2)>(tuple2));
        auto v3 = get<0>(std::forward<decltype(tuple1)>(tuple1));
        auto v4 = get<0>(std::forward<decltype(tuple2)>(tuple2));

        ASSERT_EQ(v1, v2);
        ASSERT_EQ(v3, v4);
    });

    TesterWithBuilder<2>::test("tuple_cat with 2 args", [](auto&& builder) {
        auto c1 = make_copy_counter<boq_tuple>();
        auto c2 = make_copy_counter<std_tuple>();

        auto&& [boq_tuple1, boq_tuple2] =
            builder.build(boq::Tuple{42, c1, true}, boq::Tuple{false, c1, 1.2});
        auto&& [std_tuple1, std_tuple2] =
            builder.build(std::tuple{42, c2, true}, std::tuple{false, c2, 1.2});

        auto boq_t1_2 = boq::tuple_cat(std::forward<decltype(boq_tuple1)>(boq_tuple1),
                                       std::forward<decltype(boq_tuple2)>(boq_tuple2));
        auto std_t1_2 = std::tuple_cat(std::forward<decltype(std_tuple1)>(std_tuple1),
                                       std::forward<decltype(std_tuple2)>(std_tuple2));

        static_for<0, tuple_size_v<decltype(boq_t1_2)>>(
            [&](auto i) { ASSERT_EQ(get<i.value>(boq_t1_2), get<i.value>(std_t1_2)); });
    });

    TesterWithBuilder<2>::test("tuple_cat", [](auto&& builder) {
        auto c1 = make_copy_counter<boq_tuple>();
        auto c2 = make_copy_counter<std_tuple>();

        // varying value category of first and last tuple by constructing them with the builder
        auto&& [boq_tuple1, boq_tuple3] =
            builder.build(boq::Tuple{42, c1, true}, boq::Tuple{7, 'c'});
        boq::Tuple boq_tuple2{false, c1, 1.2};
        auto&& [std_tuple1, std_tuple3] =
            builder.build(std::tuple{42, c2, true}, std::tuple{7, 'c'});
        std::tuple std_tuple2{false, c2, 1.2};

        auto boq_t1_2_3 =
            boq::tuple_cat(std::forward<decltype(boq_tuple1)>(boq_tuple1), std::move(boq_tuple2),
                           std::forward<decltype(boq_tuple3)>(boq_tuple3));
        auto std_t1_2_3 =
            std::tuple_cat(std::forward<decltype(std_tuple1)>(std_tuple1), std::move(std_tuple2),
                           std::forward<decltype(std_tuple3)>(std_tuple3));

        static_for<0, tuple_size_v<decltype(boq_t1_2_3)>>(
            [&](auto i) { ASSERT_EQ(get<i.value>(boq_t1_2_3), get<i.value>(std_t1_2_3)); });
    });

    static_assert(tuple_size_v<Tuple<int, bool>> == 2);
    static_assert(tuple_size_v<Tuple<int, bool, float, double>> == 4);

    TesterWithBuilder<1>::test("tuple_transform", [](auto&& builder) {
        auto c = make_copy_counter<boq_tuple>();

        auto&& tup = builder.build(Tuple{42, c, 12U});
        c.reset();

        auto tup2 = transform(std::forward<decltype(tup)>(tup), []<typename T>(T&& t) {
            constexpr bool is_integral = std::is_integral_v<std::remove_cvref_t<T>>;
            if constexpr (is_integral) {
                return int(t) + 2;
            } else {
                return t.stats;
            }
        });
        // Tuple<int, CopyCounter, unsigned int> => Tuple<int, CopyStats, int>
        // Tuple{42, c, 12U} => Tuple{44, c.stats, 14}

        static_assert(
            std::is_same_v<std::remove_cvref_t<decltype(tup2)>, Tuple<int, CopyStats, int>>);
        ASSERT_EQ(get<0>(tup2), 44);
        ASSERT_EQ(get<1>(tup2), (CopyStats{0, 0, 0}));
        ASSERT_EQ(get<2>(tup2), 14);
    });

    TesterWithBuilder<1>::test("tuple_filter", [](auto&& builder) {
        auto c = make_copy_counter<boq_tuple>();

        auto&& tup = builder.build(Tuple{42, 2.3F, c, 3.4, 12U});
        c.reset();

        auto tup2 = filter<std::is_integral>(std::forward<decltype(tup)>(tup));

        static_assert(
            std::is_same_v<std::remove_cvref_t<decltype(tup2)>, Tuple<int, unsigned int>>);
        ASSERT_EQ(get<0>(tup2), 42);
        ASSERT_EQ(get<1>(tup2), 12);
        ASSERT_EQ(c.stats, (CopyStats{0, 0, 0}));
    });

    TesterWithBuilder<1>::test("tuple_filter2", [](auto&& builder) {
        auto c = make_copy_counter<boq_tuple>();
        static constexpr size_t ref_tuple = 3;
        auto c2 = make_copy_counter<ref_tuple>();

        auto&& tup = builder.build(Tuple{42, 2.3F, c, 3.4, 12U});
        auto&& ref_tup = builder.build(Tuple{42, 2.3F, c2, 3.4, 12U});

        auto tup2 = filter<boq::not_<std::is_integral>::type>(std::forward<decltype(tup)>(tup));
        auto ref_tup2 = std::forward<decltype(ref_tup)>(ref_tup);

        static_assert(std::is_same_v<std::remove_cvref_t<decltype(tup2)>,
                                     Tuple<float, IndexedCopyCounter<boq_tuple>, double>>);
        ASSERT_EQ(get<0>(tup2), 2.3F);
        ASSERT_EQ(get<1>(tup2), get<2>(ref_tup2));
        ASSERT_EQ(get<2>(tup2), 3.4);
    });

    // Homework step 1: Update filter so the predicate is executed with the correct type.
    // In the current implementation we use the transform function to pass the elements to a lambda.
    // This has the inherent problem that the lambda is unable to distinguish whether the element
    // type is a value or a lvalue reference.
    //
    // Hint: add a tuple_element_t alias using the at function from Metaprogramming.h. Instead of
    // using transform, create a similar function that executes the predicate on the tuple_element_t
    // at each index instead of using the decltype of the passed element.
    Tester::test("tuple_filter_pred_passed_correct_types", []() {
        int i = 10;
        Tuple<int, int&, int> tup{4, i, 11};

        auto tup2 = filter<boq::not_<std::is_reference>::type>(std::forward<decltype(tup)>(tup));
        // static_assert(std::is_same_v<decltype(tup2), Tuple<int, int>>);
        ASSERT((std::is_same_v<decltype(tup2), Tuple<int, int>>));
    });

    return 0;
}
