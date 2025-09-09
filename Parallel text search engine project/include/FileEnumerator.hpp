// include/FileEnumerator.hpp
#pragma once
#include <string>
#include <vector>
#include "SearchConfig.hpp"

/*
“Generates” a list of files to extract – accepts initial paths (files/folders),
expands to a list of files, including recursion if required.
*/
namespace FileEnumerator {
    // Returns a list of files (from paths/folders), with/without recursion (according to cfg)
    std::vector<std::string> expand_paths(const SearchConfig& cfg);
}
