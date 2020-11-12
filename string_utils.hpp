
#pragma once

// std
#include <string>
#include <vector>

namespace util {

/// trim whitespace from the start and end of \a s
std::string trim(std::string s);

/// split \a s on char \a c
std::vector<std::string> split(const std::string& s, char c = ',');

}
