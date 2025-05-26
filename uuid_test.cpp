
#include "uuid.hpp"

// std
#include <cstring>
#include <iostream>
#include <sstream>

// local
#include "literals.hpp"
#include "simple_unit_test.hpp"

namespace util::test {

    template <size_t N>
    std::ostream& operator<<(std::ostream& os, const std::array<char, N>& a)
    {
        char buffer[N+1];
        strncpy(buffer, &a[0], N);
        buffer[N] = 0;

        os << buffer;
        return os;
    }

}

int main(int argc, char* argv[])
{
    using namespace util;
    using namespace util::test;
    using namespace std::literals;

    // check default constructed is zero'd out
    uuid u;
    ASSERT_EQUAL( u.data, decltype(uuid::data){0} );

    // check to_string
    ASSERT_EQUAL( u.to_string(), "00000000-0000-0000-0000-000000000000"s );

    // check accessors
    uint64_t v {};
    v = 0x01000000;         memcpy( &u.data[0],  &v, 4 );
    v = 0x0200;             memcpy( &u.data[4],  &v, 2 );
    v = 0x0300;             memcpy( &u.data[6],  &v, 2 );
    v = 0x04;               memcpy( &u.data[8],  &v, 1 );
    v = 0x05;               memcpy( &u.data[9],  &v, 1 );
    v = 0x0000060000000000; memcpy( &u.data[10], &v, 6 );

    ASSERT_EQUAL( u.to_string(), "00000001-0002-0003-0405-000000000006"s );
    ASSERT_EQUAL( u.time_low(), 1 );
    ASSERT_EQUAL( u.time_mid(), 2 );
    ASSERT_EQUAL( u.time_hi_version(), 3 );
    ASSERT_EQUAL( u.clock_seq_hi_reserved(), 4 );
    ASSERT_EQUAL( u.clock_seq_low(), 5 );
    ASSERT_EQUAL( u.node(), 6 );

    return failed_tests;
}
