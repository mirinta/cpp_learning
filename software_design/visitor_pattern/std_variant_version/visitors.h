#pragma once

#include <iostream>

#include "shapes.h"

struct ShapeRender
{
    void operator()(const Circle&) const { std::cout << "Render Circle" << '\n'; }
    void operator()(const Square&) const { std::cout << "Render Square" << '\n'; }
};

struct ShapeSerializer
{
    void operator()(const Circle&) const { std::cout << "Serialize Circle" << '\n'; }
    void operator()(const Square&) const { std::cout << "Serialize Square" << '\n'; }
};