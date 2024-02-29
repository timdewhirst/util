
#include "raii.hpp"

// std
#include <iostream>

// local
#include "simple_unit_test.hpp"

namespace util::test {

    class A
    {
    public:
        A() { ++static_count; }
        ~A() { --static_count; }

        static int static_count;
    };

    int A::static_count = 0;

}

int main(int argc, char* argv[])
{
    using namespace util;
    using namespace util::test;

    // check we can tidyup
    {
        auto a = new A{};        
        {
            RAII< A* > guard( a, [](auto a){ delete a; });
            ASSERT_EQUAL( a, guard.managed() );
            ASSERT_EQUAL( A::static_count, 1 );
        }
        ASSERT_EQUAL( A::static_count, 0 );
    }

    // check we can move
    {
        auto a = new A{};        
        {
            RAII< A* > guard( a, [](auto a){ delete a; });
            ASSERT_EQUAL( a, guard.managed() );
            ASSERT_EQUAL( A::static_count, 1 );

            auto moved_guard{ std::move(guard) };
            ASSERT_EQUAL( a, moved_guard.managed() );
            ASSERT_EQUAL( (A*)nullptr, guard.managed() );
            ASSERT_EQUAL( A::static_count, 1 );
        }
        ASSERT_EQUAL( A::static_count, 0 );
    }

    return failed_tests;
}
