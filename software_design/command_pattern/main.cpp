#include <iostream>

#include "Calculator.h"

int main()
{
    Calculator calculator{};
    calculator.compute(std::make_unique<Add>(5));      // result = 5
    calculator.compute(std::make_unique<Add>(4));      // result = 9
    calculator.compute(std::make_unique<Subtract>(3)); // result = 6
    calculator.compute(std::make_unique<Subtract>(2)); // result = 4
    calculator.undoLast();                             // result = 6
    std::cout << "final result = " << calculator.result() << '\n';
}