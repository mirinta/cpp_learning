#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>

template <typename T>
class ConcurrentQueue
{
public:
    explicit ConcurrentQueue() : _head(std::make_unique<Node>()), _tail(_head.get()) {}

    void enqueue(T value)
    {
        auto node = std::make_unique<Node>(std::move(value));
        std::lock_guard<std::mutex> lock(_tailMutex);
        if (_stop)
            throw std::runtime_error("Enqueue on stopped queue");

        _tail->next = std::move(node);
        _tail = _tail->next.get();
        _cv.notify_one();
    }

    bool dequeue(T& value)
    {
        std::unique_lock<std::mutex> lock(_headMutex);
        _cv.wait(lock, [this]() { return _stop || _head.get() != getTail(); });
        if (_head.get() == getTail())
            return false;

        value = std::move(_head->next->value);
        _head = std::move(_head->next);
        return true;
    }

    void stop()
    {
        std::lock_guard<std::mutex> lockHead(_headMutex);
        std::lock_guard<std::mutex> lockTail(_tailMutex);
        _stop = true;
        _cv.notify_all();
    }

private:
    struct Node
    {
        T value;
        std::unique_ptr<Node> next;
        explicit Node() = default;
        explicit Node(T val) : value(std::move(val)) {}
    };

    Node* getTail()
    {
        std::lock_guard<std::mutex> lock(_tailMutex);
        return _tail;
    }

private:
    std::unique_ptr<Node> _head;
    Node* _tail;
    std::mutex _headMutex;
    std::mutex _tailMutex;
    std::condition_variable _cv;
    bool _stop{false};
};