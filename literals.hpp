
#pragma once

// std
#include <assert.h>
#include <limits>
#include <type_traits>

#include "exception_builder.hpp"

namespace util {

    template <
        typename T,
        typename = std::enable_if_t<std::is_integral_v<T>>
        >
    constexpr T check_and_cast(unsigned long long int i)
    {
        return 
            assert(i >= std::numeric_limits<T>::lowest()), 
            assert(i <= std::numeric_limits<T>::max()),
            (T)i;
    }
}


constexpr inline uint8_t operator ""_u8(unsigned long long int i)
{
    return util::check_and_cast<uint8_t>(i);
}

constexpr inline uint16_t operator ""_u16(unsigned long long int i)
{
    return util::check_and_cast<uint16_t>(i);
}

constexpr inline uint32_t operator ""_u32(unsigned long long int i)
{
    return util::check_and_cast<uint32_t>(i);
}

constexpr inline uint64_t operator ""_u64(unsigned long long int i)
{
    return util::check_and_cast<uint64_t>(i);
}

constexpr inline size_t operator""_sz(unsigned long long int i)
{
    return (size_t)i;
}

