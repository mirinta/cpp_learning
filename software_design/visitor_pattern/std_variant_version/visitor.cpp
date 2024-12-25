#include <iostream>
#include <variant>
#include <vector>

class Circle
{
public:
    explicit Circle(double) {};
};

class Square
{
public:
    explicit Square(double) {};
};

using Shape = std::variant<Circle, Square>;

struct ShapeRender
{
    void operator()(const Circle&) const { std::cout << "Render Circle" << '\n'; }
    void operator()(const Square&) const { std::cout << "Render Square" << '\n'; }
};

struct ShapeSerializer
{
    void operator()(const Circle&) const { std::cout << "Serialize Circle" << '\n'; }
    void operator()(const Square&) const { std::cout << "Serialize Square" << '\n'; }
};

void renderShapes(const std::vector<Shape>& shapes)
{
    ShapeRender render;
    for (const auto& s : shapes) {
        std::visit(render, s);
    }
}

void serializeShapes(const std::vector<Shape>& shapes)
{
    ShapeSerializer serializer;
    for (const auto& s : shapes) {
        std::visit(serializer, s);
    }
}

int main()
{
    std::vector<Shape> shapes;
    shapes.emplace_back(Circle{3.1});
    shapes.emplace_back(Square{2.5});
    renderShapes(shapes);
    serializeShapes(shapes);
}
