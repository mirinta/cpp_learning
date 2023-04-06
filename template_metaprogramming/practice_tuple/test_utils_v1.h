#pragma once

#include <iostream>

struct Counter
{
    int default_constructs = 0;
    int copy_constructs = 0;
    int copy_assignments = 0;
    int move_constructs = 0;
    int move_assignments = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Counter& counter)
{
    os << "| default_constructs: " << counter.default_constructs << ", "
       << "copy_constructs: " << counter.copy_constructs << ", "
       << "copy_assignments: " << counter.copy_assignments << ", "
       << "move_constructs: " << counter.move_constructs << ", "
       << "move_assignments: " << counter.move_assignments << "|";
    return os;
}

struct TestClass
{
    inline static Counter counter;
    static Counter reset()
    {
        Counter old;
        using namespace std;
        std::swap(old, counter);
        return old;
    };

    TestClass() { counter.default_constructs++; };
    TestClass(const TestClass&) { counter.copy_constructs++; }
    TestClass& operator=(const TestClass&)
    {
        counter.copy_assignments++;
        return *this;
    }
    TestClass(TestClass&&) noexcept { counter.move_constructs++; }
    TestClass& operator=(TestClass&&) noexcept
    {
        counter.move_assignments++;
        return *this;
    }
};
