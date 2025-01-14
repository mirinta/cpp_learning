#pragma once

#include <iostream>

#include "shapes.h"

class OpenGLCircleStrategy
{
public:
    explicit OpenGLCircleStrategy() = default;
    void operator()(const Circle&) const { std::cout << "Draw Circle using OpenGL" << '\n'; }
};

class MetalCircleStrategy
{
public:
    explicit MetalCircleStrategy() = default;
    void operator()(const Circle&) const { std::cout << "Draw Circle using Metal" << '\n'; }
};

class OpenGLSquareStrategy
{
public:
    explicit OpenGLSquareStrategy() = default;
    void operator()(const Square&) const { std::cout << "Draw Square using OpenGL" << '\n'; }
};

class MetalSquareStrategy
{
public:
    explicit MetalSquareStrategy() = default;
    void operator()(const Square&) const { std::cout << "Draw Square using Metal" << '\n'; }
};
