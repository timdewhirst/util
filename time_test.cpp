
#include "time.hpp"

#if defined(TEST_MAIN)

// std
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <vector>
#include <thread>

// local
#include "simple_unit_test.hpp"
#include "raii.hpp"


namespace util::test {

    uint64_t get_epoch_from_system()
    {
        FILE* f = popen( "date \"+%s\"", "r" );
        util::RAII< FILE* > guard( f, [](FILE* f) { pclose(f); } );

        char buffer[128] = {};
        fread( &buffer[0], std::size(buffer), 1, f );
        return std::strtoull( &buffer[0], nullptr, 10 );
    }

    template <typename T,
              typename A,
              template<typename, typename> class C,
              typename = std::enable_if_t<
                  std::is_same_v<C<T, A>, std::list<T, A>> ||
                  std::is_same_v<C<T, A>, std::vector<T, A>>>>
    std::string join(const C<T, A>& container, std::string separator = ", ")
    {
        std::stringstream result;
        result << "[";
        const char* ps = nullptr;
        for ( const auto& t : container )
        {
            result << ( ps ? ps : "") << t;
            ps = separator.data();
        }
        result << "]";

        return result.str();
    }

}

namespace std {
    template <typename T,
              typename A,
              template<typename, typename> class C,
              typename = std::enable_if_t<
                  std::is_same_v<C<T, A>, std::list<T, A>> ||
                  std::is_same_v<C<T, A>, std::vector<T, A>>>>
    ostream& operator<<(ostream& os, const C<T, A>& container)
    {
        using namespace util::test;
        return os << join(container, ", ");
    }
}
//

int main(int argc, char* argv[])
{
    using namespace util::test;
    using namespace std::literals;

    // check we match the system time
    ASSERT_EQUAL( get_epoch_from_system(), util::get_ns_since_epoch()/1000000000 );

    // check we get different and successive numbers
    std::vector<uint64_t> times(10000);
    std::set<uint64_t> unique_times;
    for (size_t i=0; i<times.size(); ++i)
    {
        times[i] = util::get_ns_since_epoch();
        unique_times.insert(times[i]);
        std::this_thread::sleep_for(10ns);
    }

    ASSERT_EQUAL( times.size(), unique_times.size() );
    std::vector sorted_times{ times };
    std::ranges::sort(sorted_times);
    ASSERT_EQUAL( times, sorted_times );

    return failed_tests;
}

#endif
