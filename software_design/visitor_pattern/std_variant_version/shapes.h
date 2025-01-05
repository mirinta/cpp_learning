#pragma once

#include <variant>

class Circle
{
public:
    explicit Circle(double) {};
};

class Square
{
public:
    explicit Square(double) {};
};

using Shape = std::variant<Circle, Square>;