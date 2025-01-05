#pragma once

#include <iostream>

class AbstractVisitor
{
public:
    virtual ~AbstractVisitor() = default;
};

template <typename T>
class Visitor
{
public:
    virtual ~Visitor() = default;
    virtual void visit(const T&) const = 0;
};

class Circle;
class Square;
class ShapeRender : public AbstractVisitor, public Visitor<Circle>, public Visitor<Square>
{
public:
    void visit(const Circle&) const override { std::cout << "Render Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Render Square" << '\n'; }
};

class ShapeSerializer : public AbstractVisitor, public Visitor<Circle>, public Visitor<Square>
{
public:
    void visit(const Circle&) const override { std::cout << "Serialize Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Serialize Square" << '\n'; }
};