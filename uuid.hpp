
#pragma once

// std
#include <array>
#include <iosfwd>
#include <string>

namespace util {

    /// following rfc4122 (https://datatracker.ietf.org/doc/html/rfc4122):
    ///
    ///  0                   1                   2                   3
    ///  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |                          time_low                             |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |       time_mid                |         time_hi_and_version   |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |clk_seq_hi_res |  clk_seq_low  |         node (0-1)            |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |                         node (2-5)                            |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    ///
    /// string is output little-endian in format:
    ///
    /// aabbccdd-eeff-gghh-iijj-kkllmmnnoopp
    struct uuid
    {
        std::array<char, 16> data = {};

        uint32_t time_low() const;
        uint16_t time_mid() const;
        uint16_t time_hi_version() const;
        uint8_t clock_seq_hi_reserved() const;
        uint8_t clock_seq_low() const;
        uint64_t node() const;                 ///< only low 48 bits are set

        static uuid create_v4();
        std::string to_string() const;
    };
}


std::ostream& operator<<(std::ostream&, const util::uuid&);
