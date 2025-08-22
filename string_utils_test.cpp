
#include "string_utils.hpp"

#if defined(TEST_MAIN)

// std
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// local
#include "simple_unit_test.hpp"

namespace util::test {

    using stringvector_t = std::vector<std::string>;
    using stringviewvector_t = std::vector<std::string_view>;

    template <typename T, typename A>
    std::ostream& operator<<(std::ostream& os, const std::vector<T, A>& v)
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
    ASSERT_EQUAL( ltrim(" trim"), "trim"s );
    ASSERT_EQUAL( rtrim(" trim"), " trim"s );
    ASSERT_EQUAL( trim("trim "), "trim"s );
    ASSERT_EQUAL( ltrim("trim "), "trim "s );
    ASSERT_EQUAL( rtrim("trim "), "trim"s );
    ASSERT_EQUAL( trim(" trim "), "trim"s );
    ASSERT_EQUAL( trim("   \t\n"), ""s );
    ASSERT_EQUAL( trim("  foo bar zot  "), "foo bar zot"s );
    ASSERT_EQUAL( split("foo", ','), stringviewvector_t({"foo"}) );
    ASSERT_EQUAL( split("", ','), stringviewvector_t{} );
    ASSERT_EQUAL( split("foo,bar,zot", ','), stringviewvector_t({"foo", "bar", "zot"}) );
    ASSERT_EQUAL( split(",,", ','), stringviewvector_t({"", "", ""}) );
    ASSERT_EQUAL( split(",foo,", ','), stringviewvector_t({"", "foo", ""}) );

    return failed_tests;
}

#endif
