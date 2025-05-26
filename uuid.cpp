
#include "uuid.hpp"

// std
#include <bit>
#include <cstring>
#include <iostream>
#include <limits>
#include <random>

// local
#include "hex.hpp"
#include "literals.hpp"
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

    /// utility to swap endian-ness of a value
    template <typename T>
    constexpr T swap_endian(T v);

    template <>
    constexpr uint8_t swap_endian(uint8_t v) { return v; }

    template <>
    constexpr uint16_t swap_endian(uint16_t v) { return (v >> 8) | (v << 8); }

    template <>
    constexpr uint32_t swap_endian(uint32_t v)
    {
        uint32_t result = swap_endian<uint16_t>(v >> 16);
        result |= (uint32_t)swap_endian<uint16_t>(v & 0xffff) << 16;
        return result;
    }

    template <>
    constexpr uint64_t swap_endian(uint64_t v)
    {
        uint64_t result = swap_endian<uint32_t>(v >> 32);
        result |= (uint64_t)swap_endian<uint32_t>(v & 0xffffffff) << 32;
        return result;
    }


    /// convert to big endian from native byte order or vice versa
    /// - in memory data is BE
    /// - if e.g. we have a non-standard field width we have to adjust before converting endian
    /// - e.g. in case of 6 bytes representing 0x010203040506:
    ///   - uint8_t memory[] = { 1, 2, 3, 4, 5, 6 };
    ///   - memcpy 6 bytes to uint64_t value -> value = { 1, 2, 3, 4, 5, 6, 0, 0 };
    ///   - shift by sizeof(value) - count: value = { 0, 0, 1, 2, 3, 4, 5, 6 };
    ///   - endian convert: value = { 6, 5, 4, 3, 2, 1, 0, 0 };
    template<
        typename T,
        typename = std::enable_if_t<
            util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>
        >
    constexpr T be_convert(T v, size_t byte_count = sizeof(T))
    {
        if constexpr ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ )
        {
            // nothing to do
            return v;
        }

        v <<= ( (sizeof(T) - byte_count) << 3 );
        return swap_endian(v);
    }

    /// copy n bytes from \param data to return type
    template <
        typename T,
        typename  = std::enable_if_t<
            util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>
        >
    T copy_bytes_to_value(const char* data, size_t count)
    {
        T result {};
        memcpy(&result, data, count);
        return be_convert(result, count);
    }

    // copy \param value into an array of \param data
    template <
        typename T,
        typename  = std::enable_if_t<
            util::is_one_of_v<T, uint8_t, uint16_t, uint32_t, uint64_t>>
        >
    void copy_value_to_bytes(char* data, size_t count, T value)
    {
        value = be_convert(value);
        memcpy(data, &value, count);
    }

}

namespace util {

    /// following rfc4122:
    ///  0                   1                   2                   3
    ///  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |                           time_low                            |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |       time_mid                |         time_hi_and_version   |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |clk_seq_hi_res |  clk_seq_low  |         node (0-1)            |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /// |                         node (2-5)                            |
    /// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    uint32_t uuid::time_low() const
    {
        return copy_bytes_to_value<uint32_t>( &data[0], 4 );
    }

    uint16_t uuid::time_mid() const
    {
        return copy_bytes_to_value<uint16_t>( &data[4], 2 );
    }

    uint16_t uuid::time_hi_version() const
    {
        return copy_bytes_to_value<uint16_t>( &data[6], 2 );
    }

    uint8_t uuid::clock_seq_hi_reserved() const
    {
        return copy_bytes_to_value<uint8_t>( &data[8], 1 );
    }

    uint8_t uuid::clock_seq_low() const
    {
        return copy_bytes_to_value<uint8_t>( &data[9], 1 );
    }

    uint64_t uuid::node() const
    {
        return copy_bytes_to_value<uint64_t>( &data[10], 6 );
    }

    uuid uuid::create_v4()
    {
        uuid result;
        copy_value_to_bytes(&result.data[0],  8, generate_rand<uint64_t>());
        copy_value_to_bytes(&result.data[8],  8, generate_rand<uint64_t>());

        return result;
    }

    std::string uuid::to_string() const
    {
        // aaaaaaaa-bbbb-cccc-ddee-ffffgggggggg - 4 spacers + 32 chars
        std::string result(36, 0);

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
