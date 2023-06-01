
#pragma once

// std
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <typeinfo>

namespace util::test {

    /// default equality check, proxies to operator==
    template <typename T>
    inline bool equal(const T& lhs, const T& rhs)
    {
        return lhs == rhs;
    }

    static int total_tests = 0;
    static int passed_tests = 0;
    static int failed_tests = 0;

    void report()
    {
        std::cout << "total tests: " << total_tests << ", passed: " << passed_tests << ", failed: " << failed_tests << "\n";
    }

    static int _atexit = std::atexit(report);

}

#define ASSERT_EQUAL(a, b)                                              \
    {   std::stringstream ss; ss << "failure at line " << __LINE__ << " (a: actual, e: expected)\na: [" << (a) << "]\ne: [" << (b) << "]"; \
        ++util::test::total_tests;                                      \
        if (!util::test::equal((a), (b)))                               \
        {                                                               \
            ++util::test::failed_tests;                                 \
            std::cout << ss.str() << "\n";                              \
        } else {                                                        \
            ++util::test::passed_tests;                                 \
        }                                                               \
    }

#define ASSERT_TRUE(a) ASSERT_EQUAL(a, true);
#define ASSERT_FALSE(a) ASSERT_EQUAL(a, false);

#define ASSERT_EXCEPTION(f, e) {                                        \
    std::stringstream ss; ss << "failure at line " << __LINE__ << ": expected exception of type: " << typeid(e).name() << "\n"; \
    ++util::test::total_tests;                                          \
    bool passed = false;                                                \
    try {                                                               \
        f();                                                            \
    }                                                                   \
    catch (const e& exception)                                          \
    {                                                                   \
        passed = true;                                                  \
    }                                                                   \
    if (!passed)                                                        \
    {                                                                   \
        ++util::test::failed_tests;                                     \
        std::cout << ss.str() << "\n";                                  \
    } else {                                                            \
        ++util::test::passed_tests;                                     \
    }                                                                   \
}



