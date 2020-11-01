
#pragma once

// std
#include <type_traits>

namespace util {

    /// check if \tparam Type is within \tparam OtherTypes
    template <typename Type, typename... OtherTypes>
    struct is_one_of
    {
        constexpr static bool value = (std::is_same_v<Type, OtherTypes> || ...);
    };

    /// convenience template variable initialized to is_one_of<...>::Value
    template <typename Type, typename... OtherTypes>
    inline constexpr bool is_one_of_v = is_one_of<Type, OtherTypes...>::value;

}
