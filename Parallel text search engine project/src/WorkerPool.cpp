// src/WorkerPool.cpp
#include "WorkerPool.hpp"
#include "FileScanner.hpp"

#include <algorithm>
#include <iterator>
#include <utility>
#include <cassert>

WorkerPool::WorkerPool(std::size_t n,
                       ThreadSafeQueue<std::string>& tasks,
                       const SearchConfig& cfg,
                       std::atomic<bool>& cancel)
    : n_(n ? n : 1),        // If n==0, force 1 to not create 0 threads
      tasks_(tasks),
      cfg_(cfg),
      cancel_(cancel)
{
    threads_.reserve(n_);   // Pre-allocation to the thread array
}

void WorkerPool::start() {
    for (std::size_t i = 0; i < n_; ++i) {
        // For each thread – activate worker_body()
        threads_.emplace_back([this] { this->worker_body(); });
    }
}

void WorkerPool::join() {
    for (auto& t : threads_) {
        if (t.joinable()) t.join(); // Waiting for all threads to end
    }
}

void WorkerPool::worker_body() {
    // Collect results locally per thread to reduce locks
    std::vector<SearchResult> local;
    local.reserve(256);

    //As long as it wasn't requested cancellation
    while (!cancel_.load(std::memory_order_relaxed)) {
        // Retrieve a file from the queue (blocks until there is an item or the queue is closed)
        auto pathOpt = tasks_.pop();
        if (!pathOpt) break;      // If the queue is closed and empty there is no more work
        const std::string& path = *pathOpt;

        // Scan a single file according to the config; honors cancel
        auto partial = FileScanner::scan_file(path, cfg_, cancel_);

        if (!partial.empty()) {
            //If stop_on_first is defined and we find matches – we mark the remaining threads to stop.
            if (cfg_.stop_on_first) {
                cancel_.store(true, std::memory_order_relaxed);
                // Waking up threads that are blocked on pop()
                tasks_.close();
            }
            // Move the results found to the local collection
            local.insert(local.end(),
                         std::make_move_iterator(partial.begin()),
                         std::make_move_iterator(partial.end()));
        }
    }

    // Merging local results into the central repository – under short-term lock
    if (!local.empty()) {
        std::lock_guard<std::mutex> lk(res_m_);
        // move: Both within the thread and in a merge - moves objects instead of copying them.
        results_.insert(results_.end(),
                        std::make_move_iterator(local.begin()),
                        std::make_move_iterator(local.end()));
    }
}
