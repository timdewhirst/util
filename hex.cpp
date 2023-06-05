
#include "hex.hpp"


#if defined(TEST_MAIN)

// std
#include <cstring>
#include <iostream>
#include <stdexcept>

// local
#include "simple_unit_test.hpp"


namespace util::test {

    // convenience user defined literals to ensure value
    // is of the correct type
    uint8_t operator""_u8(unsigned long long int i)
    {
        if ( i<0 || i>0xff )
            throw std::runtime_error("i out of range");

        return (uint8_t)i;
    }

    uint16_t operator""_u16(unsigned long long int i)
    {
        if ( i<0 || i>0xffff )
            throw std::runtime_error("i out of range");

        return (uint16_t)i;
    }

    uint32_t operator""_u32(unsigned long long int i)
    {
        if ( i<0 || i>0xffffffff )
            throw std::runtime_error("i out of range");

        return (uint32_t)i;
    }

    size_t operator""_sz(unsigned long long int i)
    {
        return (size_t)i;
    }

    // convenience wrappers for testing to_hex
    std::array<char, 2> to_hex(uint8_t v)
    {
        std::array<char, 2> result;
        util::to_hex( &result[0], v );
        return result;
    }

    std::array<char, 4> to_hex(uint16_t v)
    {
        std::array<char, 4> result;
        util::to_hex( &result[0], v );
        return result;
    }

    std::array<char, 8> to_hex(uint32_t v)
    {
        std::array<char, 8> result;
        util::to_hex( &result[0], v );
        return result;
    }


    // handy specialization to compare a std::array<char, N> with a c-string
    template <typename T, size_t N>
    inline bool equal(const std::array<T, N>& lhs, const char* rhs)
    {
        return strncmp(&lhs[0], rhs, N) == 0;
    }

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

//

int main(int argc, char* argv[])
{
    using namespace util::test;

    char buffer[128];

    ASSERT_EQUAL( to_hex(0x00_u8), "00" );
    ASSERT_EQUAL( to_hex(0xff_u8), "ff" );
    ASSERT_EQUAL( to_hex(0x0f_u8), "0f" );
    ASSERT_EQUAL( to_hex(0xf0_u8), "f0" );
    ASSERT_EQUAL( util::to_hex(&buffer[0], 0xf0_u8), 2_sz );

    ASSERT_EQUAL( to_hex(0x0000_u16), "0000" );
    ASSERT_EQUAL( to_hex(0x0001_u16), "0001" );
    ASSERT_EQUAL( to_hex(0x0010_u16), "0010" );
    ASSERT_EQUAL( to_hex(0x0100_u16), "0100" );
    ASSERT_EQUAL( to_hex(0x1000_u16), "1000" );
    ASSERT_EQUAL( util::to_hex(&buffer[0], 0x1000_u16), 4_sz );

    ASSERT_EQUAL( to_hex(0x00000000_u32), "00000000" );
    ASSERT_EQUAL( to_hex(0x00000001_u32), "00000001" );
    ASSERT_EQUAL( to_hex(0x00000010_u32), "00000010" );
    ASSERT_EQUAL( to_hex(0x00000100_u32), "00000100" );
    ASSERT_EQUAL( to_hex(0x00001000_u32), "00001000" );
    ASSERT_EQUAL( to_hex(0x00010000_u32), "00010000" );
    ASSERT_EQUAL( to_hex(0x00100000_u32), "00100000" );
    ASSERT_EQUAL( to_hex(0x01000000_u32), "01000000" );
    ASSERT_EQUAL( to_hex(0x10000000_u32), "10000000" );
    ASSERT_EQUAL( to_hex(0xffffffff_u32), "ffffffff" );
    ASSERT_EQUAL( util::to_hex(&buffer[0], 0xffffffff_u32), 8_sz );

    return 0;
}

#endif
