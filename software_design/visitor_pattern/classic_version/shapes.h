#pragma once

#include "visitors.h"

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void accept(const ShapeVisitor&) const = 0;
};

class Circle : public Shape
{
public:
    explicit Circle(double) {}
    void accept(const ShapeVisitor& v) const override { v.visit(*this); }
};

class Square : public Shape
{
public:
    explicit Square(double) {}
    void accept(const ShapeVisitor& v) const override { v.visit(*this); }
};