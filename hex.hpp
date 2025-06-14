
#pragma once

// std
#include <array>
#include <cstddef>
#include <cstdint>

// util
#include "traits.hpp"

namespace util {

    /// char buffer[2];
    /// to_hex($buffer[0], (uint8_t)0xaf); // -> buffer contains "af"
    ///
    /// output is big-endian i.e. 0x1234 -> "1234"
    ///
    /// \return number of bytes written
    template <typename T,
              typename = std::enable_if<
                  util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>
              >
    constexpr size_t to_hex(char* c, T v)
    {
        constexpr std::array<char, 16> hexchars = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'a', 'b', 'c', 'd', 'e', 'f'
        };

        constexpr size_t count = 2*sizeof(T);
        c += count - 1;
        for (size_t i=count; i>0; --i)
        {
            *c-- = hexchars[v & 0x0f];
            v >>= 4;
        }

        return count;
    }

}
