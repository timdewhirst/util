
#include "hexdump.hpp"

// std
#include <array>

// local
#include "hex.hpp"

namespace {

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
            util::to_hex( &result[0], offset );
            result[8] = '\n';
            return result;
        }

        // okay, constructing a line
        std::string result( 79, ' ' );

        // write the offset first
        char* p = &result[0];
        util::to_hex( p, offset );

        // write the bytes, max 16
        len = std::min(len, size_t{16});
        p = &result[10];
        for (size_t i=0; i<len; ++i)
        {
            util::to_hex( p, data[i] );
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
