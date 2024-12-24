#include <iostream>
#include <memory>
#include <vector>

class Circle;
class Square;

class ShapeVisitor
{
public:
    virtual ~ShapeVisitor() = default;

    virtual void visit(const Circle&) const = 0;
    virtual void visit(const Square&) const = 0;
};

class RenderVisitor : public ShapeVisitor
{
public:
    void visit(const Circle&) const override { std::cout << "Render Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Render Square" << '\n'; }
};

class SerializeVisitor : public ShapeVisitor
{
public:
    void visit(const Circle&) const override { std::cout << "Serialize Circle" << '\n'; }
    void visit(const Square&) const override { std::cout << "Serialize Square" << '\n'; }
};

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
    shapes.push_back(std::make_unique<Circle>(3.1));
    shapes.push_back(std::make_unique<Square>(2.5));
    renderShapes(shapes);
    serializeShapes(shapes);
}