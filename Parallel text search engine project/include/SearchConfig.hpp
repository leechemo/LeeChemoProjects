// include/SearchConfig.hpp
#pragma once
#include <string>
#include <vector>
#include <thread>

/*
SearchConfig is an object that centralizes all the settings for a single run of the search engine –
 what to search for, where to search, in what form, and in what format to return the results.
*/
struct SearchConfig {
    // Input sources for search: files/folders
    std::vector<std::string> paths;

    // What to search
    std::string pattern;
    bool use_regex     = false;
    bool ignore_case   = false;

    // File collection
    bool recursive     = true;
    bool skip_binary   = true;                 // Skip binary files
    std::vector<std::string> exts;             // List of allowed extensions. If empty – we will include everything.

    // Parallelism
    std::size_t workers = std::thread::hardware_concurrency();

    // Stopping when something is found first
    bool stop_on_first = false;

    // Output: "table" | "csv" | "json"
    std::string out_format = "table";
};
