// include/Util.hpp
#pragma once
#include <string>
#include <vector>
#include <algorithm>

/*
Its purpose is to centralize small, common utility functions
that the code uses over and over again
*/
namespace Util {
    // Convert to lowercase (simple ASCII)
    inline std::string to_lower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
        return s;
    }

    // Checks if the path has one of the allowed extensions (if the list is empty – returns true)
    bool has_extension(const std::string& path,
                       const std::vector<std::string>& allowed);
}
