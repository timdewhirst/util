
#include "raii.hpp"

// std
#include <iostream>

// local
#include "simple_unit_test.hpp"

namespace util::test {

    static int static_a_count = 0;

    class A
    {
    public:
        A() { ++static_a_count; }
        ~A() { --static_a_count; }
    };

}

int main(int argc, char* argv[])
{
    using namespace util;
    using namespace util::test;

    // check we can tidyup
    auto a = new A{};        
    {
        RAII< A* > guard( a, [](auto a){ delete a; });
        ASSERT_EQUAL( a, guard.managed() );
        ASSERT_EQUAL( static_a_count, 1 );
    }
    ASSERT_EQUAL( static_a_count, 0 );

    return failed_tests;
}
