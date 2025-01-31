#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <thread>

#include "ConcurrentQueue.h"

class ThreadPool
{
public:
    explicit ThreadPool(size_t numOfThreads = std::thread::hardware_concurrency())
    {
        _workers.reserve(numOfThreads);
        for (size_t i = 0; i < numOfThreads; ++i) {
            _workers.emplace_back([this] { work(); });
        }
    }

    ~ThreadPool()
    {
        _stop = true;
        _queue.stop();
        for (auto& worker : _workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    template <typename F, typename... Args>
    auto push(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>
    {
        using return_type = std::invoke_result_t<F, Args...>;
        if (_stop)
            throw std::runtime_error("Push on stopped ThreadPool!");

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> result = task->get_future();
        _queue.enqueue([task]() { (*task)(); });
        return result;
    }

private:
    void work()
    {
        while (!_stop) {
            std::function<void()> task;
            if (_queue.dequeue(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

private:
    ConcurrentQueue<std::function<void()>> _queue;
    std::vector<std::thread> _workers;
    std::atomic<bool> _stop{false};
};