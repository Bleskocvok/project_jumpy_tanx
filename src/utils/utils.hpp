
#pragma once

#include <string>
#include <sstream> // std::stringstream
#include <fstream>
#include <exception>

namespace utils {

inline std::string load_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw std::runtime_error("File '" + filename + "' couldn't be loaded.");
    }
    std::stringstream stream;
    stream << in.rdbuf();
    return stream.str();
}

} // namespace utils
