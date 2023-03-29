#pragma once

#include <memory>
#include <vector>

class Shape
{
public:
    template <typename ShapeT, typename DrawFunction>
    Shape(ShapeT shape, DrawFunction drawer)
        : pimpl(std::make_unique<ShapeModel<ShapeT, DrawFunction>>(std::move(shape),
                                                                   std::move(drawer)))
    {
    }

    /// copy operations
    Shape(const Shape& other) : pimpl(other.pimpl->clone()) {}
    Shape& operator=(const Shape& other)
    {
        other.pimpl->clone().swap(pimpl); // the copy-and-swap idiom
        return *this;
    }

    /// move operations
    /// option 1: moved-from shapes are semantically equivalent to a nullptr
    /// - Shape(Shape&&) = default;
    /// - Shape& operator=(Shape&&) = default;
    ///
    /// option 2: move remains undefined, copy serves as a falback
    /// - do NOT =delete them, it will causes a compilation error if you try to move

private:
    struct ShapeConcept
    {
        virtual ~ShapeConcept() = default;
        virtual void do_draw() const = 0;
        /// use the Prototype design pattern to support copy operations
        virtual std::unique_ptr<ShapeConcept> clone() const = 0;
    };

    template <typename ShapeT, typename DrawFunction>
    struct ShapeModel : public ShapeConcept
    {
        ShapeModel(ShapeT shape, DrawFunction drawer)
            : _shape(std::move(shape)), _drawer(std::move(drawer))
        {
        }
        void do_draw() const override { _drawer(_shape); }
        std::unique_ptr<ShapeConcept> clone() const override
        {
            return std::make_unique<ShapeModel>(*this);
        }

        ShapeT _shape;
        DrawFunction _drawer;
    };

    /// this function is not part of the class,
    /// it is a free function and injected into the surrounding namespace
    friend void draw(const Shape& shape) { shape.pimpl->do_draw(); }

    std::unique_ptr<ShapeConcept> pimpl;
};

inline void drawShapes(const std::vector<Shape>& shapes)
{
    for (const auto& shape : shapes) {
        draw(shape);
    }
}

/// some independent shapes defined by users:
struct Circle
{
    float radius{0};
};

struct Square
{
    float side{0};
};

inline void exampleUsage()
{
    std::vector<Shape> shapes{};
    shapes.emplace_back(Circle{4.56}, [](const Circle& c) {
        std::printf("draw a circle with radius %.2f.\n", c.radius);
    });
    shapes.emplace_back(Square{7.89}, [](const Square& s) {
        std::printf("draw a square with side %.2f.\n", s.side);
    });
    drawShapes(shapes);
}
