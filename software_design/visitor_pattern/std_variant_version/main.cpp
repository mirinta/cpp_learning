#include <vector>

#include "visitors.h"

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
