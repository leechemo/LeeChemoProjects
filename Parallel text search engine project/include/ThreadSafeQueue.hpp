// include/ThreadSafeQueue.hpp
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <utility>

/*
This is a generic thread-safe queue (Producer/Consumer).
In our project it holds the search tasks (file paths).
Several threads (“workers”) read from it concurrently: each worker fetches Path, scans the file, and returns to fetch more,
without Busy-Waiting and without collisions.
*/

template <typename T>
class ThreadSafeQueue {
public:
    // Insert a task/value into the queue
    void push(T value) {
        {
            std::lock_guard<std::mutex> lk(m_);
            q_.push(std::move(value));
        }
        cv_.notify_one();
    }

    // Fetch a value; blocks until there is a value in queue or the queue is closed and empty.
    // Consumers wait for the bell until there is a task or the queue closes.
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [this]{ return closed_ || !q_.empty(); }); // if the condition isn't met- thread will temporarily release the lock and go to sleep.
        if (q_.empty()) return std::nullopt;
        T v = std::move(q_.front());
        q_.pop();
        return v;
    }

    // Signals that there are no more insertions; wakes up all those waiting.
    void close() {
        {
            std::lock_guard<std::mutex> lk(m_);
            closed_ = true;
        }
        cv_.notify_all();
    }

private:
    std::queue<T> q_;
    std::mutex m_;
    std::condition_variable cv_; //A bell for synchronization between threads.
    bool closed_ = false;
};
