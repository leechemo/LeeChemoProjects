 // include/WorkerPool.hpp
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstddef>
#include <string>

#include "ThreadSafeQueue.hpp"
#include "SearchConfig.hpp"
#include "SearchResult.hpp"

/*
WorkerPool – Worker Pool (Threads):
Create N threads (start()),
each of which will pull a task (file path) from a shared queue, scan the file (using FileScanner), save results locally,
and finally merge the results into a central structure,
then allow the main thread to wait for them all to finish (join()),
and retrieve all results via results().
*/
class WorkerPool {
public:
    WorkerPool(std::size_t n,
               ThreadSafeQueue<std::string>& tasks,
               const SearchConfig& cfg,
               std::atomic<bool>& cancel);

    void start();  // Creates the threads and starts working- Each one runs worker_body()
    void join();   // Waiting for everyone to finish.

    const std::vector<SearchResult>& results() const { return results_; }

private:
    void worker_body(); // What each thread do

    std::size_t n_; //Number of threads in the pool
    ThreadSafeQueue<std::string>& tasks_; // File paths to read from
    const SearchConfig& cfg_;
    std::atomic<bool>& cancel_;

    std::vector<std::thread> threads_;

    // Merging results at the end of each thread's work
    std::mutex res_m_;
    std::vector<SearchResult> results_;
};
