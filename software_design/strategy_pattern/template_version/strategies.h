#pragma once

#include <iostream>

class Circle;
class Square;

template <typename T>
class DrawStrategy
{
public:
    virtual ~DrawStrategy() = default;
    virtual void draw(const T&) const = 0;
};

class OpenGLCircleStrategy : public DrawStrategy<Circle>
{
public:
    void draw(const Circle&) const override { std::cout << "Draw Circle using OpenGL" << '\n'; }
};

class MetalCircleStrategy : public DrawStrategy<Circle>
{
public:
    void draw(const Circle&) const override { std::cout << "Draw Circle using Metal" << '\n'; }
};

class OpenGLSquareStrategy : public DrawStrategy<Square>
{
public:
    void draw(const Square&) const override { std::cout << "Draw Square using OpenGL" << '\n'; }
};

class MetalSquareStrategy : public DrawStrategy<Square>
{
public:
    void draw(const Square&) const override { std::cout << "Draw Square using Metal" << '\n'; }
};
