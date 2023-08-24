
#include "hexdump.hpp"

// std
#include <iostream>

// local
#include "simple_unit_test.hpp"

int main(int argc, char* argv[])
{
    using namespace std::string_literals;
    using namespace util::test;

    uint8_t data[] = "\0\1\2\3\4hello, world!";
    ASSERT_EQUAL( hexdump(&data[0], 0),  "00000000\n"s );
    ASSERT_EQUAL( hexdump(&data[0], 4),  "00000000  00 01 02 03                                       |....|\n"s );
    ASSERT_EQUAL( hexdump(&data[0], 16), "00000000  00 01 02 03 04 68 65 6c  6c 6f 2c 20 77 6f 72 6c  |.....hello, worl|\n"s );
    ASSERT_EQUAL( hexdump(&data[0], 19), "00000000  00 01 02 03 04 68 65 6c  6c 6f 2c 20 77 6f 72 6c  |.....hello, worl|\n"
                                         "00000010  64 21 00                                          |d!.|\n"s );

    return failed_tests;
}
