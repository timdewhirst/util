
#pragma once

// std
#include <chrono>

#include <string.h>
#include <time.h>


// local
#include "exception_builder.hpp"

namespace util {

/// return a uint64_t representing nanoseconds since epoch
uint64_t get_ns_since_epoch()
{
#if defined( __unix__ ) || defined( __APPLE__ )
    timespec ts;
    if ( clock_gettime(CLOCK_REALTIME, &ts) != 0 )
        exception_builder<std::runtime_error>() << "failed to get time: "
                                                << strerror(errno);

    return ts.tv_sec * 1'000'000'000 + ts.tv_nsec;
#endif
}

}
