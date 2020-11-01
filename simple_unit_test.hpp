
#pragma once

// std
#include <iostream>
#include <sstream>


/// default equality check, proxies to operator==
template <typename T>
inline bool equal(const T& lhs, const T& rhs)
{
    return lhs == rhs;
}

#define ASSERT_EQUAL(a, b)                                              \
    {   std::stringstream ss; ss << "failure at line " << __LINE__ << "(a: actual, e: expected)\na: " << (a) << "\ne: " << (b); \
        if (!equal((a), (b)))                                           \
            std::cout << ss.str() << "\n";                              \
    }



