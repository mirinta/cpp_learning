class CalculatorCommand
{
public:
    virtual ~CalculatorCommand() = default;

    virtual int execute(int val) const = 0;
    virtual int undo(int val) const = 0;
};

class Add : public CalculatorCommand
{
public:
    explicit Add(int operand) : operand(operand) {}

    int execute(int val) const override { return val + operand; }
    int undo(int val) const override { return val - operand; }

private:
    int operand;
};

class Subtract : public CalculatorCommand
{
public:
    explicit Subtract(int operand) : operand(operand) {}

    int execute(int val) const override { return val - operand; }
    int undo(int val) const override { return val - operand; }

private:
    int operand;
};