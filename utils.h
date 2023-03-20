#pragma once

#include <iostream>

/**
 * @note C++23 provides std::println.
 */
void println(const char* c);

class TestClass
{
public:
    TestClass() { println("constructor"); }
    ~TestClass() { println("desctructor"); }

    TestClass(const TestClass&) { println("copy constructor"); }
    TestClass& operator=(const TestClass&)
    {
        println("copy-assignment operator");
        return *this;
    }

    TestClass(TestClass&&) noexcept { println("move constructor"); }
    TestClass& operator=(TestClass&&) noexcept
    {
        println("move-assignment operator");
        return *this;
    }
};
