// include/SearchResult.hpp
#pragma once
#include <string>
#include <cstddef>

/*
SearchResult describes one result: where (file/row/column) and what (match)
 + all the text of the row.
*/
struct SearchResult {
    std::string file;  // The path to the file where the occurrence is located.
    std::size_t line_no; // The line number in the file where the occurrence was found. 1-based
    std::size_t col;     // The starting position of the match, 0-based index
    std::string match;   // The found text (or regex group)
    std::string line;    // The full line for display
};
