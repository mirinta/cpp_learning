#pragma once

#include "visitors.h"

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void accept(const AbstractVisitor&) const = 0;
};

class Circle : public Shape
{
public:
    explicit Circle(double) {}
    void accept(const AbstractVisitor& v) const override
    {
        if (const auto* const cv = dynamic_cast<const Visitor<Circle>* const>(&v)) {
            cv->visit(*this);
        }
    }
};

class Square : public Shape
{
public:
    explicit Square(double) {}
    void accept(const AbstractVisitor& v) const override
    {
        if (const auto* const cv = dynamic_cast<const Visitor<Square>*>(&v)) {
            cv->visit(*this);
        }
    }
};