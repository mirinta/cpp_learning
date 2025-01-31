#include <memory>
#include <vector>

#include "shapes.h"

void renderShapes(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    ShapeRender render;
    for (const auto& s : shapes) {
        s->accept(render);
    }
}

void serializeShapes(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    ShapeSerializer serializer;
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
