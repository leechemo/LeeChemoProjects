// src/FileScanner.cpp
#include "FileScanner.hpp"
#include "Util.hpp"

#include <fstream>
#include <regex>
#include <string>
#include <iterator>

namespace FileScanner {

    // Rough check for a binary file: if there is a NULL among the first 512 bytes -> probably binary
    bool looks_binary(const std::string& path) {
        std::ifstream in(path, std::ios::binary);
        if (!in) return false; //If it doesn't open, we won't be stamped "binary"
        char buf[512];
        in.read(buf, sizeof(buf));
        std::streamsize n = in.gcount();
        for (std::streamsize i = 0; i < n; ++i) {
            if (buf[i] == '\0') return true;
        }
        return false;
    }

    // Finds all occurrences of needle within line (regular text search)
    // If searched_line == line -> case-sensitive search (there's different between upper or lower case)
    // If searched_line == lower(line) -> case-insensitive search (to_lower(line))
    static void find_all_occurrences(const std::string& line,
                                     const std::string& searched_line,
                                     const std::string& needle,
                                     std::size_t line_no,
                                     const std::string& file,
                                     std::vector<SearchResult>& acc,
                                     bool stop_on_first)
    {
        std::size_t pos = 0;
        while (true) {
            pos = searched_line.find(needle, pos);
            if (pos == std::string::npos) break;

            // The match shown to the user will be taken from the original line (preserves case in ignore-case mode)
            std::string matched = line.substr(pos, needle.size());

            acc.push_back(SearchResult{file, line_no, pos, matched, line});
            if (stop_on_first) return;

            // With ++pos – overlaps are allowed.
            ++pos;
        }
    }

    std::vector<SearchResult> scan_file(const std::string& path,
                                        const SearchConfig& cfg,
                                        std::atomic<bool>& cancel)
    {
        std::vector<SearchResult> results;

        if (cfg.pattern.empty()) return results;

        // Skip binaries if defined
        if (cfg.skip_binary && looks_binary(path)) {
            return results;
        }

        std::ifstream in(path);
        if (!in) {
            // Not opened/no permission – returns empty
            return results;
        }

        // Preparing the needle and the options
        std::string needle= cfg.pattern;
        std::regex re;
        bool use_regex = cfg.use_regex;
        bool ignore_case = cfg.ignore_case;

        if (use_regex) {
            auto flags = std::regex_constants::ECMAScript;
            if (ignore_case) flags |= std::regex_constants::icase;
            try {
                re = std::regex(needle, flags);
            } catch (const std::regex_error&) {
                // Invalid expression – returns no results instead of aborting the process
                return results;
            }
        } else if (ignore_case) {
            needle = Util::to_lower(needle);
        }

        std::string line;
        std::size_t line_no = 0;

        // Line-by-line reading: saves memory and works well with most texts
        while (!cancel && std::getline(in, line)) {
            ++line_no;

            if (use_regex) {
                // Regex: Find all matches in a line
                std::smatch m;
                auto begin = line.cbegin();
                while (std::regex_search(begin, line.cend(), m, re)) {
                    // The column position is the offset + position within the current suffix.
                    std::size_t col = static_cast<std::size_t>(std::distance(line.cbegin(), begin)) +
                                      static_cast<std::size_t>(m.position());
                    results.push_back(SearchResult{path, line_no, col, m.str(), line});

                    if (cfg.stop_on_first) return results;
                    begin = m.suffix().first; // Continuing to look ahead in line
                }
            } else {
                // Regular text search (case-sensitive/insensitive)
                if (ignore_case) {
                    std::string lower_line = Util::to_lower(line);
                    find_all_occurrences(line, lower_line, needle, line_no, path, results, cfg.stop_on_first);
                    if (cfg.stop_on_first && !results.empty()) return results;
                } else {
                    find_all_occurrences(line, line, needle, line_no, path, results, cfg.stop_on_first);
                    if (cfg.stop_on_first && !results.empty()) return results;
                }
            }
        }

        return results;
    }

}
