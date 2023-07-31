
#include "uuid.hpp"

// std
#include <cstring>
#include <iostream>
#include <limits>
#include <random>

// local
#include "hex.hpp"
#include "traits.hpp"

namespace {

    static std::random_device& rd()
    {
        static std::random_device static_rd;
        return static_rd;
    }

    static std::mt19937& gen()
    {
        static std::mt19937 static_gen( rd()() );
        return static_gen;
    }


    template <
        typename T,
        typename  = std::enable_if_t<
            util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>
        >
    T generate_rand()
    {
        static std::uniform_int_distribution<T> distribution(
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max()
            );
        return distribution( gen() );
    }

    // copy n bytes from \param data to return type
    template <
        typename T,
        typename  = std::enable_if_t<
            util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>
        >
    T copy_bytes_to_value(const char* data, size_t count = sizeof(T))
    {
        T result {};
        memcpy(&result, data, count);
        return result;
    }

    // copy \param value into an array of \param data
    template <
        typename T,
        typename  = std::enable_if_t<
            util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>
        >
    void copy_value_to_bytes(char* data, T value, size_t count = sizeof(T))
    {
        memcpy(data, &value, count);
    }

}

namespace util {

    /// following rfc4122:
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
    uint32_t uuid::time_low() const
    {
        return copy_bytes_to_value<uint32_t>( &data[0] );
    }

    uint16_t uuid::time_mid() const
    {
        return copy_bytes_to_value<uint16_t>( &data[4] );
    }

    uint16_t uuid::time_hi_version() const
    {
        return copy_bytes_to_value<uint16_t>( &data[6] );
    }

    uint8_t uuid::clock_seq_hi_reserved() const
    {
        return copy_bytes_to_value<uint8_t>( &data[8] );
    }

    uint8_t uuid::clock_seq_low() const
    {
        return copy_bytes_to_value<uint8_t>( &data[9] );
    }

    uint64_t uuid::node() const
    {
        return copy_bytes_to_value<uint64_t>( &data[10], 6 );
    }

    uuid uuid::create_v4()
    {
        uuid result;
        copy_value_to_bytes(&result.data[0],  generate_rand<uint32_t>());
        copy_value_to_bytes(&result.data[4],  generate_rand<uint16_t>());
        copy_value_to_bytes<uint16_t>(&result.data[6], (generate_rand<uint16_t>() & 0x0fff) | 0x4000);
        copy_value_to_bytes<uint16_t>(&result.data[8], generate_rand<uint16_t>() & 0x3fff + 0x8000);
        copy_value_to_bytes(&result.data[10], generate_rand<uint64_t>(), 6);

        return result;
    }

    std::string uuid::to_string() const
    {
        // aabbccdd-eeff-gghh-iijj-kkllmmnnoopp - 4 spacers + 32 chars
        std::string result(41, 0);

        char* p = &result[0];
        for ( size_t i=0; i<16; ++i )
        {
            p += to_hex(p, data[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9)
                *p++ = '-';
        }

        return result;
    }

}

std::ostream& operator<<(std::ostream& os, const util::uuid& uuid)
{
    return os << uuid.to_string();
}
