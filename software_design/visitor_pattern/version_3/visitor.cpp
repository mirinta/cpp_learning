#include <iostream>
#include <memory>
#include <vector>

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

class RenderVisitor : public AbstractVisitor, public Visitor<Circle>, public Visitor<Square>
{
public:
    void visit(const Circle&) const override { std::cout << "Render Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Render Square" << '\n'; }
};

class SerializeVisitor : public AbstractVisitor, public Visitor<Circle>, public Visitor<Square>
{
public:
    void visit(const Circle&) const override { std::cout << "Serialize Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Serialize Square" << '\n'; }
};

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
        if (const auto* const cv = dynamic_cast<const Visitor<Square>* const>(&v)) {
            cv->visit(*this);
        }
    }
};

void renderShapes(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    RenderVisitor render;
    for (const auto& s : shapes) {
        s->accept(render);
    }
}

void serializeShapes(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    SerializeVisitor serializer;
    for (const auto& s : shapes) {
        s->accept(serializer);
    }
}

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.emplace_back(std::make_unique<Circle>(3.1));
    shapes.emplace_back(std::make_unique<Square>(2.5));
    renderShapes(shapes);
    serializeShapes(shapes);
    return 0;
}
