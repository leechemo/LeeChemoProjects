// src/main.cpp
#include "SearchConfig.hpp"
#include "ThreadSafeQueue.hpp"
#include "FileEnumerator.hpp"
#include "WorkerPool.hpp"
#include "OutputWriter.hpp"
#include "Util.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <atomic>
#include <cstdlib>

// Simple CLI usage/help text
static void print_usage(const char* argv0) {
    std::cout << R"(Usage:)" << argv0 << R"( --pattern <text|regex> [options] [paths...]

Options:
  -p, --pattern <str>       Search string or regular expression (required)
  -i, --ignore-case         Case-insensitive search
  -R, --recursive           Recursively scan directories
      --regex               Interpret pattern as ECMAScript regular expression
      --skip-binary         Skip binary files (default: enabled)
      --no-skip-binary      Do not skip binary files
      --stop-on-first       Stop after the first match (across all files)
  -j, --threads <N>         Number of worker threads (default: min(#files, hw))
      --ext "<e1,e2,...>"   Filter by extensions (e.g. ".cpp,.h,.txt")
      --format <table|csv|json>
                            Output format (default: table)
  -h, --help                Show this help and exit

Paths:
  If no paths are given, the current directory "." is used.
)" << std::endl;
}

// Split a comma-separated string into tokens (no trimming)
static std::vector<std::string> split_csv(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == ',') {
            if (!cur.empty()) out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

int main(int argc, char** argv) {
    // Initialize default search configuration
    SearchConfig cfg{};
    cfg.use_regex     = false;
    cfg.ignore_case   = false;
    cfg.recursive     = false;
    cfg.skip_binary   = true;   // default: skip binary files
    cfg.stop_on_first = false;
    cfg.out_format    = "table";
    cfg.paths.clear();
    cfg.exts.clear();

    std::size_t threads_override = 0; // 0 means "auto"

    // Very simple argument parsing (no fancy library; order-sensitive)
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;

        } else if (arg == "-p" || arg == "--pattern") {
            if (i + 1 >= argc) { std::cerr << "Missing value for --pattern\n"; print_usage(argv[0]); return 1; }
            cfg.pattern = argv[++i];

        } else if (arg == "--regex") {
            cfg.use_regex = true;

        } else if (arg == "-i" || arg == "--ignore-case") {
            cfg.ignore_case = true;

        } else if (arg == "-R" || arg == "--recursive") {
            cfg.recursive = true;

        } else if (arg == "--skip-binary") {
            cfg.skip_binary = true;

        } else if (arg == "--no-skip-binary") {
            cfg.skip_binary = false;

        } else if (arg == "--stop-on-first") {
            cfg.stop_on_first = true;

        } else if (arg == "-j" || arg == "--threads") {
            if (i + 1 >= argc) { std::cerr << "Missing value for --threads\n"; print_usage(argv[0]); return 1; }
            try {
                threads_override = static_cast<std::size_t>(std::stoul(argv[++i]));
            } catch (...) {
                std::cerr << "Invalid value for --threads\n";
                return 1;
            }

        } else if (arg == "--ext") {
            if (i + 1 >= argc) { std::cerr << "Missing value for --ext\n"; print_usage(argv[0]); return 1; }
            cfg.exts = split_csv(argv[++i]);
            // Normalize extensions to lowercase for case-insensitive comparison
            for (auto& e : cfg.exts) e = Util::to_lower(e);

        } else if (arg == "--format" || arg == "--out-format") {
            if (i + 1 >= argc) { std::cerr << "Missing value for --format\n"; print_usage(argv[0]); return 1; }
            cfg.out_format = argv[++i];

        } else if (!arg.empty() && arg[0] == '-') {
            // Unknown flag
            std::cerr << "Unknown option: " << arg << "\n";
            print_usage(argv[0]);
            return 1;

        } else {
            // Positional argument -> path (file or directory)
            cfg.paths.push_back(arg);
        }
    }

    // Validate required arguments
    if (cfg.pattern.empty()) {
        std::cerr << "Error: --pattern is required.\n";
        print_usage(argv[0]);
        return 1;
    }
    if (cfg.paths.empty()) {
        // Default to current directory if no paths were provided
        cfg.paths.push_back(".");
    }

    // Step 1: Expand paths (files/directories) into a flat list of files
    auto all_files = FileEnumerator::expand_paths(cfg);

    // Step 2: Push files into the task queue, optionally filtering by extension
    ThreadSafeQueue<std::string> tasks;
    std::size_t pushed = 0;
    for (const auto& f : all_files) {
        if (Util::has_extension(f, cfg.exts)) {
            tasks.push(f);
            ++pushed;
        }
    }
    // No more tasks will be produced
    tasks.close();

    if (pushed == 0) {
        // Nothing to search (no files found or all filtered out) → print empty output and exit
        OutputWriter::write({}, cfg);
        return 0;
    }

    // Determine number of workers: override, else min(#files, hardware_concurrency)
    std::size_t hw = std::thread::hardware_concurrency();
    if (hw == 0) hw = 4; // conservative fallback if the value is unknown
    std::size_t n_workers = threads_override ? threads_override
                                             : std::min<std::size_t>(pushed, hw);

    std::atomic<bool> cancel{false};

    // Step 3: Start the worker pool (threads consume file paths and scan them)
    WorkerPool pool(n_workers, tasks, cfg, cancel);
    pool.start();
    pool.join(); // Wait for all workers to finish

    // Step 4: Produce output (table/csv/json)
    const auto& results = pool.results();
    OutputWriter::write(results, cfg);

    return 0;
}
