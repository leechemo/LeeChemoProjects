// src/Util.cpp
#include "Util.hpp"
#include <filesystem>
#include <string>
#include <algorithm>

namespace Util {

    bool has_extension(const std::string& path,
                       const std::vector<std::string>& allowed)
    {
        // If no allowed extensions are defined – everything is allowed.
        if (allowed.empty()) return true;

        // Extract the extension from the path (including the dot, e.g. ".cpp")
        std::filesystem::path p(path);
        const std::string ext = p.extension().string();  // If there is no extension ""

        // Case-insensitive comparison: Lowercase both the suffix and the list.
        const std::string lower_ext = to_lower(ext);

        for (const std::string& e : allowed) {
            if (lower_ext == to_lower(e)) {
                return true; // match found
            }
        }
        return false; // no match
    }

}
