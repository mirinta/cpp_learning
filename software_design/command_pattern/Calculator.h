#include <memory>
#include <stack>

#include "calculator_commands.h"

class Calculator
{
public:
    explicit Calculator() = default;

    void compute(std::unique_ptr<CalculatorCommand> command)
    {
        current = command->execute(current);
        stack.push(std::move(command));
    }

    void undoLast()
    {
        if (stack.empty())
            return;

        auto command = std::move(stack.top());
        stack.pop();
        current = command->undo(current);
    }

    int result() const { return current; }

    void clear()
    {
        current = 0;
        std::stack<std::unique_ptr<CalculatorCommand>>{}.swap(stack);
    }

private:
    int current{0};
    std::stack<std::unique_ptr<CalculatorCommand>> stack;
};