#include <coroutine>

#include "spdlog/spdlog.h"

auto _ = []() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][process %P][thread %t][%l] %v");
    return true;
}();

struct Coro
{
    struct promise_type
    {
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        Coro get_return_type() { return std::coroutine_handle<promise_type>::from_promise(*this); }
        void return_void() {}
        void unhandled_exception() {}
    };

    Coro(std::coroutine_handle<promise_type> handle) : handle{handle} {}

    std::coroutine_handle<promise_type> handle;
};

int main() { return 0; }