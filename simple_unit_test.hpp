
#pragma once

// std
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <type_traits>

namespace util::test {

    /// default equality check, proxies to operator==
    template <
      typename V,
      typename E,
      typename = std::enable_if_t<
        std::is_convertible_v<E, V>
      >
    >
    inline bool equal(const V& v, const E& e)
    {
        return v == (V)e;
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

#define ASSERT_EQUAL(a, b) {                                            \
    auto __a = (a);                                                     \
    auto __b = (b);                                                     \
    std::stringstream ss; ss << "failure at line " << __LINE__ << " (a: actual, e: expected)\na: [" << __a << "]\ne: [" << __b << "]"; \
    ++util::test::total_tests;                                          \
    if (util::test::equal(__a, __b))                                    \
    {                                                                   \
        ++util::test::passed_tests;                                     \
    } else {                                                            \
        ++util::test::failed_tests;                                     \
        std::cout << ss.str() << "\n";                                  \
    }                                                                   \
    }

#define ASSERT_NEQUAL(a, b) {                                           \
    auto __a = (a);                                                     \
    auto __b = (b);                                                     \
    std::stringstream ss; ss << "failure at line " << __LINE__ << " (a: actual, e: expected)\na: [" << __a << "]\ne: [" << __b << "]"; \
    ++util::test::total_tests;                                          \
    if (!util::test::equal(__a, __b))                                   \
    {                                                                   \
        ++util::test::passed_tests;                                     \
    } else {                                                            \
        ++util::test::failed_tests;                                     \
        std::cout << ss.str() << "\n";                                  \
    }                                                                   \
    }

#define ASSERT_TRUE(a) ASSERT_EQUAL(a, true)
#define ASSERT_FALSE(a) ASSERT_EQUAL(a, false)

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



