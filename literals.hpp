
#pragma once

// std
#include <limits>
#include <type_traits>

#include "exception_builder.hpp"

namespace util {

    template <
        typename T,
        typename = std::enable_if_t<std::is_integral_v<T>>
        >
    T check_and_cast(unsigned long long int i)
    {
        if ( i < std::numeric_limits<T>::lowest() || std::numeric_limits<T>::max() < i )
            exception_builder<std::runtime_error>() << "argument is outside of valid range: " << i;

        return (T)i;
    }
}


inline uint8_t operator ""_u8(unsigned long long int i)
{
    return util::check_and_cast<uint8_t>(i);
}

inline uint16_t operator ""_u16(unsigned long long int i)
{
    return util::check_and_cast<uint16_t>(i);
}

inline uint32_t operator ""_u32(unsigned long long int i)
{
    return util::check_and_cast<uint32_t>(i);
}

inline uint64_t operator ""_u64(unsigned long long int i)
{
    return util::check_and_cast<uint64_t>(i);
}

inline size_t operator""_sz(unsigned long long int i)
{
    return (size_t)i;
}

