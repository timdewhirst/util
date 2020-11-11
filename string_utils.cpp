
#include "string_utils.hpp"

// std
#include <algorithm>
#include <cctype>

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

/// split \a s on char \a c
std::vector<std::string> split(const std::string& s, char c)
{
    if ( s.empty() )
        return {};

    std::vector<std::string> result;

    auto I = std::begin(s);
    const auto E = std::end(s);

    std::string current;
    while (true)
    {
        if (*I == c || I == E)
        {
            result.push_back(current);
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


#if defined(TEST_MAIN)

// std
#include <cstring>
#include <iostream>

// local
#include "simple_unit_test.hpp"

using stringvector = std::vector<std::string>;

std::ostream& operator<<(std::ostream& os, const stringvector& v)
{
    static const char separator[] = ", ";
    const char* separator_p = "";
    for (auto I=std::begin(v); I!=std::end(v); ++I)
    {
        os << separator_p << *I;
        separator_p = separator;
    }

    return os;
}

//
int main(int argc, char* argv[])
{
    using namespace std::string_literals;
    using namespace util::test;

    ASSERT_EQUAL( trim("trim"), "trim"s );
    ASSERT_EQUAL( trim(" trim"), "trim"s );
    ASSERT_EQUAL( trim("trim "), "trim"s );
    ASSERT_EQUAL( trim(" trim "), "trim"s );
    ASSERT_EQUAL( trim("   \t\n"), ""s );
    ASSERT_EQUAL( trim("  foo bar zot  "), "foo bar zot"s );
    ASSERT_EQUAL( split("foo,bar,zot"), stringvector({"foo", "bar", "zot"}) );
    ASSERT_EQUAL( split(",,"), stringvector({"", "", ""}) );
    ASSERT_EQUAL( split(",foo,"), stringvector({"", "foo", ""}) );

    return 0;
}

#endif
