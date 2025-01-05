#include "shapes.h"

void drawAllShapes(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    for (const auto& shape : shapes) {
        shape->draw();
    }
}

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.emplace_back(std::make_unique<Circle>(3.14, std::make_unique<OpenGLCircleStrategy>()));
    shapes.emplace_back(std::make_unique<Square>(4.56, std::make_unique<OpenGLSquareStrategy>()));
    drawAllShapes(shapes);
}