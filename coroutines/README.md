# Basics

## Why the coroutines classes must define ```promise_type```?

The compiler's view of a coroutine function (simplified):

```C++
Coro f() {
    Coro::promise_type p();
    Coro coro_obj = p.get_return_object();
    try {
        co_await p.initial_suspend();
        // ************************************************************
        // * The actual function body of f() written by the developer *
        // ************************************************************
    } catch(...) {
        p.unhandled_exception();
    }
    co_await p.final_suspend();
}
```

# Resources

## Videos

- [Cppcon 2022: Deciphering C++ Coroutines](https://www.youtube.com/watch?v=J7fYddslH0Q&t=2381s)

- [Cppcon 2022: C++20's Coroutines for Beginners](https://www.youtube.com/watch?v=8sEe-4tig_A&t=1154s)

- [CppNow 2023: Introduction to C++ Coroutines Through a Thread Scheduling Demonstration](https://www.youtube.com/watch?v=kIPzED3VD3w)
  
- [CppCon 2024: C++ Coroutines and Structured Concurrency in Practice](https://www.youtube.com/watch?v=aPqMQ7SXSiw)

- [CpponSea 2022: Understanding C++ Coroutines by Example, Part 1](https://www.youtube.com/watch?v=tj0URCY_A1s)

## Libraries

- [cppcoro](https://github.com/lewissbaker/cppcoro)