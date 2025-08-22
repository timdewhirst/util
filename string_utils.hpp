
#pragma once

// std
#include <cctype>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

namespace util {

    /// trim whitespace from the end of \a s
    constexpr std::string_view rtrim(std::string_view s)
    {
        size_t i=0;
        for ( auto I = s.crbegin(); std::isspace(*I) && I != s.crend(); ++I )
            ++i;

        return s.substr(0, s.size() - i);
    }

    /// trim whitespace from the start of \a s
    constexpr std::string_view ltrim(std::string_view s)
    {
        size_t i=0;
        for ( auto I = s.cbegin(); std::isspace(*I) && I != s.cend(); ++I )
            ++i;

        return s.substr(i, s.size() - i);
    }

    /// trim whitespace from the start and end of \a s
    constexpr std::string_view trim(std::string_view s)
    {
        return ltrim(rtrim(s));
    }

    /// split \a s on char \a c; this will continue until the
    /// end of s:
    ///
    /// - split("a-bcd-ef-g", '-') -> ["a", "bcd", "ef", "g"]
    /// - split("a-bcd", '-') -> ["a", "bcd"]
    /// - split("a", '-') -> ["a"]
    std::vector<std::string_view> split(std::string_view s, char c)
    {
        if ( s.empty() )
            return {};

        std::vector<std::string_view> result;

        size_t p = 0, i = 0;
        while (true)
        {
            if (s[i] == c || i == s.size())
            {
                result.push_back(s.substr(p, i-p));
                if ( i == s.size() )
                    break;

                p = ++i;
            }
            else
            {
                ++i;
            }
        }

        return result;
    }

}
