#include <numbers>
#include <string>

class Circle;
class Square;

class AbstractShapeVisitor
{
public:
    virtual ~AbstractShapeVisitor() = default;

    virtual void visit(Circle*) const = 0;
    virtual void visit(Square*) const = 0;
};

class AbstractShape
{
public:
    virtual ~AbstractShape() = default;

    virtual double area() const = 0;
    virtual void accept(AbstractShapeVisitor*) = 0;
};

class Circle : public AbstractShape
{
public:
    explicit Circle(double radius) : _radius(radius) {}

    double radius() const { return _radius; }
    double area() const override { return std::numbers::pi_v<double> * _radius * _radius; }

    void accept(AbstractShapeVisitor* visitor) override { return visitor->visit(this); }

private:
    double _radius;
};

class Square : public AbstractShape
{
public:
    explicit Square(double side) : _side(side) {}

    double side() const { return _side; }
    double area() const override { return _side * _side; }

    void accept(AbstractShapeVisitor* visitor) override { return visitor->visit(this); }

private:
    double _side;
};

class SerializeShapeVisitor : public AbstractShapeVisitor
{
public:
    void visit(Circle* circle) const override
    {
        std::printf("Serializing Circle, Radius = %f, Area = %f\n", circle->radius(),
                    circle->area());
    }

    void visit(Square* square) const override
    {
        std::printf("Serializing Circle, Side = %f, Area = %f\n", square->side(), square->area());
    }
};

class RenderShapeVisitor : public AbstractShapeVisitor
{
public:
    void visit(Circle* circle) const override
    {
        std::printf("Rendering Circle, Radius = %f, Area = %f\n", circle->radius(), circle->area());
    }

    void visit(Square* square) const override
    {
        std::printf("Rendering Square, Side = %f, Area = %f\n", square->side(), square->area());
    }
};
