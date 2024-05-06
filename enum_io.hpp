
#pragma once

// std
#include <iostream>
#include <string_view>
#include <tuple>

/// must be called with:
/// - first argument is enum type
/// - subsequent arguments are a tuple literal of enum values to literal string
///
/// will allocate static constexpr storage of a c-style array of tuples; this
/// is searched linearly for enums when callint to_string
#define DECLARE_ENUM_IO( E, ... )                                       \
    static constexpr std::tuple<E, const char*> E##__enum_io_storage[] = { \
        __VA_ARGS__                                                     \
    };                                                                  \
                                                                        \
    [[maybe_unused]] static constexpr std::string_view to_string(E e)   \
    {                                                                   \
        for (size_t i=0; i<std::size(E##__enum_io_storage); ++i)        \
        {                                                               \
            const auto& [enum_value, enum_string] = E##__enum_io_storage[i]; \
            if (enum_value == e)                                        \
                return enum_string;                                     \
        }                                                               \
                                                                        \
        return to_string(E::UNKNOWN);                                   \
    }                                                                   \
                                                                        \
    [[maybe_unused]] static std::ostream& operator<<(std::ostream& os, E e) \
    {                                                                   \
        return os << to_string(e);                                      \
    }
