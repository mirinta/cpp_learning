#include <memory>
#include <variant>
#include <vector>

class Square
{
public:
    explicit Square() = default;
};

class Circle
{
public:
    explicit Circle() = default;
};

using Shape = std::variant<Square, Circle>;

struct Draw
{
    void operator()(const Square&) {}
    void operator()(const Circle&) {}
};

using Shapes = std::vector<Shape>;
void drawShapes(const Shapes& shapes)
{
    for (const auto& s : shapes) {
        std::visit(Draw{}, s);
    }
}

int as_main()
{
    Shapes shapes;
    shapes.emplace_back(Square{});
    shapes.emplace_back(Circle{});
    drawShapes(shapes);
    return 0;
}
