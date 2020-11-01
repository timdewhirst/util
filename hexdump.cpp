
#include "hexdump.hpp"

// std
#include <array>

// local
#include "traits.hpp"

namespace {

    constexpr std::array<char, 16> hexchars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    // char buffer[2];
    // to_hex((uint8_t)0xaf, &buffer[0]); // -> "af"
    template <typename T,
              typename = std::enable_if<util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>>
    void to_hex(T v, char* c)
    {
        constexpr size_t count = 2*sizeof(T);
        c += count - 1;
        for (size_t i=count; i>0; --i)
        {
            *c-- = hexchars[v & 0x0f];
            v >>= 4;
        }
    }

    char as_printable(uint8_t c)
    {
        if (c < 0x20)
            return '.';
        if (c > 0x7e)
            return '.';

        return c;
    }

    // \a offset is the offset into the total data block
    // \a data is the data for this line i.e. data[0] is the first piece of data at \a offset
    std::string construct_line(uint32_t offset, uint8_t* data, size_t len)
    {
        // line is 8 + 2 + (16*3) + 2 + 2 + 16 = 78 chars wide
        //           1         2         3    3    4         5         6         7      7
        // 0         0         0         0    5    0         0         0         0      7
        // xxxxxxxx  00 11 22 33 44 55 66 77  88 99 aa bb cc dd ee ff  |................|

        // if no data, write the offset only
        if (!data || len == 0)
        {
            // write offset as hex (fixed width - 8 chars), end with newline
            std::string result( 9, ' ' );
            to_hex(offset, &result[0]);
            result[8] = '\n';
            return result;
        }

        // okay, constructing a line
        std::string result( 79, ' ' );

        // write the offset first
        char* p = &result[0];
        to_hex(offset, p);

        // write the bytes, max 16
        len = std::min(len, size_t{16});
        p = &result[10];
        for (size_t i=0; i<len; ++i)
        {
            to_hex(data[i], p);
            p += 3;
            if (i == 7) p += 1; // extra gap between octets
        }

        // write the clear text version
        p = &result[60];
        *p++ = '|';
        for (size_t i=0; i<len; ++i)
            *p++ = as_printable(data[i]);
        *p++ = '|';
        *p++ = '\n';
        *p++ = '\0';

        // cut off any trailing spaces
        result.resize(result.size() - (16 - len));

        return result;
    }

};


std::string hexdump(uint8_t* data, size_t len)
{
    std::string result;

    size_t offset = 0;

    do
    {
        size_t to_write = std::min(len, size_t{16});
        result += construct_line(offset, data + offset, to_write);
        offset += to_write;
        len -= to_write;
    }
    while (len != 0);

    return result;
}


#if defined(TEST_MAIN)

// std
#include <cstring>
#include <iostream>

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

    // convenience wrappers for testing to_hex
    std::array<char, 2> to_hex(uint8_t v)
    {
        std::array<char, 2> result;
        ::to_hex(v, &result[0]);
        return result;
    }

    std::array<char, 4> to_hex(uint16_t v)
    {
        std::array<char, 4> result;
        ::to_hex(v, &result[0]);
        return result;
    }

    std::array<char, 8> to_hex(uint32_t v)
    {
        std::array<char, 8> result;
        ::to_hex(v, &result[0]);
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
    using namespace std::string_literals;
    using namespace util::test;

    ASSERT_EQUAL( to_hex(0x00_u8), "00" );
    ASSERT_EQUAL( to_hex(0xff_u8), "ff" );
    ASSERT_EQUAL( to_hex(0x0f_u8), "0f" );
    ASSERT_EQUAL( to_hex(0xf0_u8), "f0" );

    ASSERT_EQUAL( to_hex(0x0000_u16), "0000" );
    ASSERT_EQUAL( to_hex(0x0001_u16), "0001" );
    ASSERT_EQUAL( to_hex(0x0010_u16), "0010" );
    ASSERT_EQUAL( to_hex(0x0100_u16), "0100" );
    ASSERT_EQUAL( to_hex(0x1000_u16), "1000" );

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

    uint8_t data[] = "\0\1\2\3\4hello, world!";
    ASSERT_EQUAL( hexdump(&data[0], 0),  "00000000\n"s );
    ASSERT_EQUAL( hexdump(&data[0], 4),  "00000000  00 01 02 03                                       |....|\n"s );
    ASSERT_EQUAL( hexdump(&data[0], 16), "00000000  00 01 02 03 04 68 65 6c  6c 6f 2c 20 77 6f 72 6c  |.....hello, worl|\n"s );
    ASSERT_EQUAL( hexdump(&data[0], 19), "00000000  00 01 02 03 04 68 65 6c  6c 6f 2c 20 77 6f 72 6c  |.....hello, worl|\n"
                                         "00000010  64 21 00                                          |d!.|\n"s );

    return 0;
}

#endif
