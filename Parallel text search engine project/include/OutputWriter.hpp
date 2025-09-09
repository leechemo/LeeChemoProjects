// include/OutputWriter.hpp
#pragma once
#include <vector>
#include "SearchResult.hpp"
#include "SearchConfig.hpp"

/*
Define the system's output layer interface.
Responsible for printing/saving the results according to the requested format (table/CSV/JSON).
*/
namespace OutputWriter {
    // Writes the results to standard output
    void write(const std::vector<SearchResult>& results,
               const SearchConfig& cfg);
}
