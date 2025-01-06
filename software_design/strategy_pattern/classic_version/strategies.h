#pragma once

#include <iostream>

class Circle;
class Square;

class DrawCircleStrategy
{
public:
    virtual ~DrawCircleStrategy() = default;
    virtual void draw(const Circle&) const = 0;
};

class OpenGLCircleStrategy : public DrawCircleStrategy
{
public:
    void draw(const Circle&) const override { std::cout << "Draw Circle using OpenGL" << '\n'; }
};

class MetalCircleStrategy : public DrawCircleStrategy
{
public:
    void draw(const Circle&) const override { std::cout << "Draw Circle using Metal" << '\n'; }
};

class DrawSquareStrategy
{
public:
    virtual ~DrawSquareStrategy() = default;
    virtual void draw(const Square&) const = 0;
};

class OpenGLSquareStrategy : public DrawSquareStrategy
{
public:
    void draw(const Square&) const override { std::cout << "Draw Square using OpenGL" << '\n'; }
};

class MetalSquareStrategy : public DrawSquareStrategy
{
public:
    void draw(const Square&) const override { std::cout << "Draw Square using Metal" << '\n'; }
};
