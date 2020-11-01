
// std
#include <cstring>

// local
#include "simple_unit_test.hpp"

namespace util::test {

    // custom equality check for char arrays with length 4
    bool equal(const char (&a)[4], const char (&b)[4])
    {
        return strncmp(a, b, 4) == 0;
    }

}

int main(int argc, char* argv[])
{
    ASSERT_EQUAL(1, 1);
    ASSERT_EQUAL(1, 2);
    ASSERT_EQUAL("foo!", "foo!"); // comparing pointers

    {
        const char a[] = "foo!";
        const char b[] = "foo!";
        ASSERT_EQUAL(a, b);         // note this failure; unexpected!
    }

    {
        const char a[] = "foo";
        const char b[] = "foo";
        ASSERT_EQUAL(a, b);         // this now passes thanks to the custom equal function
    }

    return 0;
}
