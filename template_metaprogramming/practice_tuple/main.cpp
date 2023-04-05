#include <iostream>

#include "TestClass.h"
#include "Tuple.h"

int main()
{
    {
        auto tuple = mrnt::make_tuple(8, true, 3.14);
        std::cout << tuple.data << '\n'; /// 8, int

        mrnt::Tuple tuple2(1, false, 2.56); /// need deduction guide
        std::cout << tuple2.data << '\n';   /// 1, int

        /// get data from lvalue tuple
        mrnt::Tuple lvalue_tuple(0, 1, "lvalue tuple");
        std::cout << mrnt::get<2>(lvalue_tuple) << '\n';

        /// get data from const-lvalue tuple
        const mrnt::Tuple const_lvalue_tuple(0, 1, "const lvalue tuple");
        std::cout << mrnt::get<2>(const_lvalue_tuple) << '\n';

        /// get data from rvalue tuple
        std::cout << mrnt::get<2>(mrnt::Tuple{0, 1, "rvalue tuple"}) << '\n';

        /// assign data
        mrnt::get<0>(lvalue_tuple) = 999;
        std::cout << "assign new value " << mrnt::get<0>(lvalue_tuple) << '\n';

        /// test the TestClass
        TestClass::reset();
        [[maybe_unused]] TestClass t;
        std::cout << TestClass::counter << '\n'; /// 1, 0, 0
        TestClass t2 = t;
        std::cout << TestClass::counter << '\n'; /// 1, 1, 0
        t = t2;
        std::cout << TestClass::counter << '\n'; /// 1, 2, 0
        t = std::move(t2);
        std::cout << TestClass::counter << '\n'; /// 1, 2, 1

        /// count the number of copy operations, compare with the std::tuple
        std::cout << "---------- test make_tuple<> ----------" << '\n';
        TestClass::reset();
        [[maybe_unused]] auto _std_tuple = std::make_tuple(0, 1, t);
        std::cout << "std tuple: \n" << TestClass::counter << '\n'; /// #copies = 1
        TestClass::reset();
        [[maybe_unused]] auto _our_tuple = mrnt::make_tuple(0, 1, t);
        std::cout << "our tuple: \n" << TestClass::counter << '\n';

        /// getter, compare with the std::tuple
        std::cout << "---------- test get<> ----------" << '\n';
        TestClass::reset();
        [[maybe_unused]] auto _x = std::get<2>(_std_tuple);
        std::cout << "std tuple: \n" << TestClass::counter << '\n'; /// #copies = 1
        TestClass::reset();
        [[maybe_unused]] auto __x = mrnt::get<2>(_our_tuple);
        std::cout << "our tuple: \n" << TestClass::counter << '\n';
    }
    std::cout << "---------- test tuple_cat<> ----------" << '\n';
    TestClass t;
    TestClass::reset();
    auto t1 = mrnt::make_tuple(t, false, 3.14);
    auto t2 = mrnt::make_tuple(true, 2, std::string{"hello"});
    auto t3 = mrnt::make_tuple(6.1, "132", 99);
    [[maybe_unused]] auto t1_2_3 = mrnt::tuple_cat(t1, t2, t3);
    std::cout << "our tuple: \n" << TestClass::counter << '\n';
    TestClass::reset();
    auto _t1 = std::make_tuple(t, false, 3.14);
    auto _t2 = std::make_tuple(true, 2, std::string{"hello"});
    auto _t3 = std::make_tuple(6.1, "132", 99);
    [[maybe_unused]] auto _t1_2_3 = std::tuple_cat(_t1, _t2, _t3);
    std::cout << "std tuple: \n" << TestClass::counter << '\n';
    return 0;
}
