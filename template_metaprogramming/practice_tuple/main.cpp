#include <iostream>

#include "Tuple.h"

int main()
{
    auto std_tuple = std::make_tuple(0, 1, "std tuple");
    std::cout << std::get<2>(std_tuple) << '\n';

    [[maybe_unused]] auto tuple = make_tuple(8, true, 3.14);
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

    return 0;
}
