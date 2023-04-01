#include <iostream>

#include "TestClass.h"
#include "Tuple.h"

int main()
{
    auto tuple = make_tuple(8, true, 3.14);
    std::cout << tuple.data << '\n'; /// 8, int

    Tuple tuple2(1, false, 2.56);     /// need deduction guide
    std::cout << tuple2.data << '\n'; /// 1, int

    /// get data from lvalue tuple
    Tuple lvalue_tuple(0, 1, "lvalue tuple");
    std::cout << get<2>(lvalue_tuple) << '\n';

    /// get data from const-lvalue tuple
    const Tuple const_lvalue_tuple(0, 1, "const lvalue tuple");
    std::cout << get<2>(const_lvalue_tuple) << '\n';

    /// get data from rvalue tuple
    std::cout << get<2>(Tuple{0, 1, "rvalue tuple"}) << '\n';

    /// assign data
    get<0>(lvalue_tuple) = 999;
    std::cout << "assign new value " << get<0>(lvalue_tuple) << '\n';

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
    [[maybe_unused]] auto _our_tuple = make_tuple(0, 1, t);
    std::cout << "our tuple: \n" << TestClass::counter << '\n';

    /// getter, compare with the std::tuple
    std::cout << "---------- test get<> ----------" << '\n';
    TestClass::reset();
    [[maybe_unused]] auto _x = std::get<2>(_std_tuple);
    std::cout << "std tuple: \n" << TestClass::counter << '\n'; /// #copies = 1
    TestClass::reset();
    [[maybe_unused]] auto __x = get<2>(_our_tuple);
    std::cout << "our tuple: \n" << TestClass::counter << '\n';

    return 0;
}
