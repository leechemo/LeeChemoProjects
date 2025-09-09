// src/OutputWriter.cpp
#include "OutputWriter.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <cctype>
#include <cstdio>

// Internal helper functions (local file)
namespace {

    // return "path:line:col"
    std::string join_location(const SearchResult& r) {
        std::ostringstream os;
        os << r.file << ":" << r.line_no << ":" << r.col;
        return os.str();
    }

    // Escaping to CSV according to the simple rules: always mark with double quotes, inside the field doubles to ""
    std::string csv_escape(const std::string& s) {
        std::string out;
        out.reserve(s.size() + 2);
        out.push_back('"');
        for (char c : s) {
            if (c == '"') out += "\"\"";
            else out.push_back(c);
        }
        out.push_back('"');
        return out;
    }

    // Escape to JSON: quotes, backslashes, and control characters -> standard sequences; other characters are preserved
    std::string json_escape(const std::string& s) {
        std::string out;
        out.reserve(s.size() + 2);
        out.push_back('"');
        for (unsigned char c : s) {
            switch (c) {
                case '\"': out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\b': out += "\\b";  break;
                case '\f': out += "\\f";  break;
                case '\n': out += "\\n";  break;
                case '\r': out += "\\r";  break;
                case '\t': out += "\\t";  break;
                default:
                    if (c < 0x20) { // Additional control characters -> \u00XX
                        char buf[7];
                        std::snprintf(buf, sizeof(buf), "\\u%04X", c);
                        out += buf;
                    } else {
                        out.push_back(static_cast<char>(c));
                    }
            }
        }
        out.push_back('"');
        return out;
    }

    // Screen-friendly table output
    void write_table(const std::vector<SearchResult>& results) {
        std::ostream& out = std::cout;
        for (const auto& r : results) {
            out << join_location(r) << " | " << r.match << "\n";
            out << "    " << r.line << "\n";
        }
    }

    // CSV output: header + rows with proper escaping
    void write_csv(const std::vector<SearchResult>& results) {
        std::ostream& out = std::cout;
        out << "file,line,col,match,line_text\n";
        for (const auto& r : results) {
            out << csv_escape(r.file) << ","
                << r.line_no << ","
                << r.col << ","
                << csv_escape(r.match) << ","
                << csv_escape(r.line)  << "\n";
        }
    }

    // JSON output: array of {file,line,col,match,line_text} objects
    void write_json(const std::vector<SearchResult>& results) {
        std::ostream& out = std::cout;
        out << "[";
        for (std::size_t i = 0; i < results.size(); ++i) {
            const auto& r = results[i];
            if (i) out << ",";
            out << "{"
                << "\"file\":"      << json_escape(r.file)  << ","
                << "\"line\":"      << r.line_no            << ","
                << "\"col\":"       << r.col                << ","
                << "\"match\":"     << json_escape(r.match) << ","
                << "\"line_text\":" << json_escape(r.line)
                << "}";
        }
        out << "]";
        if (!results.empty()) out << "\n";
    }

}

namespace OutputWriter {

    /*
     Chooses which format to print the results in and delegates the work to the helper functions.
     */
    void write(const std::vector<SearchResult>& results,
               const SearchConfig& cfg)
    {
        const std::string& fmt = cfg.out_format;
        if (fmt == "csv") {
            write_csv(results);
        } else if (fmt == "json") {
            write_json(results);
        } else {
            write_table(results);
        }
    }

}
