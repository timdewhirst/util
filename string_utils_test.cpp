
#include "string_utils.hpp"

#if defined(TEST_MAIN)

// std
#include <cstring>
#include <iostream>

// local
#include "simple_unit_test.hpp"

namespace util::test {

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

} // end of namespace

//
int main(int argc, char* argv[])
{
    using namespace std::string_literals;
    using namespace util::test;
    using namespace util;

    ASSERT_EQUAL( trim("trim"), "trim"s );
    ASSERT_EQUAL( trim(" trim"), "trim"s );
    ASSERT_EQUAL( trim("trim "), "trim"s );
    ASSERT_EQUAL( trim(" trim "), "trim"s );
    ASSERT_EQUAL( trim("   \t\n"), ""s );
    ASSERT_EQUAL( trim("  foo bar zot  "), "foo bar zot"s );
    ASSERT_EQUAL( split("foo", ','), stringvector({"foo"}) );
    ASSERT_EQUAL( split("foo,bar,zot", ','), stringvector({"foo", "bar", "zot"}) );
    ASSERT_EQUAL( split(",,", ','), stringvector({"", "", ""}) );
    ASSERT_EQUAL( split(",foo,", ','), stringvector({"", "foo", ""}) );

    return failed_tests;
}

#endif
