#include "spdlog/spdlog.h"

#include "ThreadPool.h"

int task(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    spdlog::info("Calculating task({})...", x);
    return x * x;
}

int main()
{
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%l] %v");
    static constexpr size_t kNumOfTasks = 10;
    std::vector<std::future<int>> results;
    results.reserve(kNumOfTasks);
    ThreadPool pool(4);
    for (size_t i = 0; i < kNumOfTasks; ++i) {
        results.emplace_back(pool.push(task, i + 1));
    }
    for (auto& result : results) {
        spdlog::info(result.get());
    }
    return 0;
}
