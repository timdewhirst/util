
// std
#include <cstring>
#include <string>

// local
#include "simple_unit_test.hpp"

using namespace std::string_literals;

namespace util::test {

    // custom equality check for char arrays with length 4
    template <size_t N>
    bool equal(const char (&a)[N], const char (&b)[N])
    {
        return strncmp(a, b, N) == 0;
    }

}

int main(int argc, char* argv[])
{
    ASSERT_EQUAL(1, 1);
    ASSERT_NEQUAL(1, 2);
    ASSERT_EQUAL("foo!", "foo!");          // comparing pointers
    ASSERT_EQUAL("foozot!"s, "foozot!"s);  // comparing strings

    return 0;
}
