// include/FileScanner.hpp
#pragma once
#include <string>
#include <vector>
#include <atomic>
#include "SearchResult.hpp"
#include "SearchConfig.hpp"

/*
Defines the API for single file scanning.
*/
namespace FileScanner {
    // Rough check if a file appears to be binary (by first N bytes)
    bool looks_binary(const std::string& path);

    // Single file scan by config; respects cancel
    std::vector<SearchResult> scan_file(const std::string& path,
                                        const SearchConfig& cfg,
                                        std::atomic<bool>& cancel);
}
