#pragma once

#include <functional>

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
};

class Circle : public Shape
{
public:
    explicit Circle(double, std::function<void(const Circle&)> drawer) : drawer(std::move(drawer))
    {
    }
    void draw() const override { drawer(*this); }

private:
    std::function<void(const Circle&)> drawer;
};

class Square : public Shape
{
public:
    explicit Square(double, std::function<void(const Square&)> drawer) : drawer(std::move(drawer))
    {
    }
    void draw() const override { drawer(*this); }

private:
    std::function<void(const Square&)> drawer;
};
