
#pragma once

// std
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace util {

    /// trim whitespace from the start and end of \a s
    std::string trim(std::string s)
    {
        if ( s.empty() )
            return s;

        size_t start = 0;
        while ( std::isspace(s[start]) && start < s.size() )
            ++start;

        // short circuit - if start == s.size()-1 then the entire string was whitespace
        if ( start == s.size()-1 )
            return s;

        size_t end = s.size()-1;
        while ( std::isspace(s[end]) && end > start )
            --end;

        return s.substr(start, end-start+1);
    }

    /// split \a s on char \a c; this will continue until the
    /// end of s:
    ///
    /// - split("a-bcd-ef-g", '-') -> ["a", "bcd", "ef", "g"]
    /// - split("a-bcd", '-') -> ["a", "bcd"]
    /// - split("a", '-') -> ["a"]
    std::vector<std::string> split(const std::string& s, char c)
    {
        if ( s.empty() )
            return {};

        std::vector<std::string> result;

        auto I = s.cbegin();
        const auto E = s.cend();

        std::string current;
        while (true)
        {
            if (*I == c || I == E)
            {
                result.push_back(std::move(current));
                current = std::string();
            }
            else
            {
                current += *I;
            }

            if (I == E)
                break;

            ++I;
        }

        return result;
    }

}
