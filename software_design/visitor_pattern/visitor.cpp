#include <memory>

#include "visitor.h"

int main()
{
    auto circle = std::make_unique<Circle>(1.5);
    auto square = std::make_unique<Square>(2.4);

    auto serializer = std::make_unique<SerializeShapeVisitor>();
    circle->accept(serializer.get());
    square->accept(serializer.get());

    auto render = std::make_unique<RenderShapeVisitor>();
    circle->accept(render.get());
    square->accept(render.get());
}