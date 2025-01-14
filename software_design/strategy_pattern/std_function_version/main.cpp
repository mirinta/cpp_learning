#include "shapes.h"
#include "strategies.h"

void drawAllShapes(const std::vector<std::unique_ptr<Shape>>& shapes)
{
    for (const auto& shape : shapes) {
        shape->draw();
    }
}

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.emplace_back(std::make_unique<Circle>(1.23, OpenGLCircleStrategy()));
    shapes.emplace_back(std::make_unique<Circle>(2.34, MetalCircleStrategy()));
    shapes.emplace_back(std::make_unique<Square>(3.45, OpenGLSquareStrategy()));
    shapes.emplace_back(std::make_unique<Square>(4.56, MetalSquareStrategy()));
    drawAllShapes(shapes);
}
