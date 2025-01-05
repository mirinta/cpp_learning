#pragma once

#include <iostream>

class Circle;
class Square;
class ShapeVisitor
{
public:
    virtual ~ShapeVisitor() = default;

    virtual void visit(const Circle&) const = 0;
    virtual void visit(const Square&) const = 0;
};

class ShapeRender : public ShapeVisitor
{
public:
    void visit(const Circle&) const override { std::cout << "Render Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Render Square" << '\n'; }
};

class ShapeSerializer : public ShapeVisitor
{
public:
    void visit(const Circle&) const override { std::cout << "Serialize Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Serialize Square" << '\n'; }
};