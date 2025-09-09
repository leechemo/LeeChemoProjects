// src/FileEnumerator.cpp
#include "FileEnumerator.hpp"

#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

namespace FileEnumerator {

    std::vector<std::string> expand_paths(const SearchConfig& cfg) {
        std::vector<std::string> out;
        std::error_code ec;

        for (const auto& raw : cfg.paths) {
            fs::path root(raw);                     // The path that the user gave (file or folder)

            if (!fs::exists(root, ec)) continue;    // Skipping non-existent path/permission errors
            if (fs::is_regular_file(root, ec)) {
                out.push_back(root.string());       // If the path is a file, add and continue.
                continue;
            }

            if (fs::is_directory(root, ec)) {
                if (cfg.recursive) {
                    // Recursive scan; skips permission errors without crashing the process
                    fs::recursive_directory_iterator it(
                        root,
                        fs::directory_options::skip_permission_denied,
                        ec
                    ), end;

                    while (it != end) {
                        if (ec) {                   // In case of an error with the previous step
                            ec.clear();
                            it.increment(ec);
                            continue;
                        }
                        const fs::directory_entry& entry = *it;
                        if (entry.is_regular_file(ec)) {
                            out.push_back(entry.path().string());
                        }
                        it.increment(ec);           // Moving to the next element (with error handling)
                    }
                } else {
                    // Scan a single folder (non-recursive)
                    fs::directory_iterator it(
                        root,
                        fs::directory_options::skip_permission_denied,
                        ec
                    ), end;

                    while (it != end) {
                        if (ec) {
                            ec.clear();
                            it.increment(ec);
                            continue;
                        }
                        const fs::directory_entry& entry = *it;
                        if (entry.is_regular_file(ec)) {
                            out.push_back(entry.path().string());
                        }
                        it.increment(ec);
                    }
                }
            }
        }

        return out;
    }

}
