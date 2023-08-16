
#pragma once

// std
#include <functional>
#include <type_traits>

namespace util {

    /// a generalized wrapper for RAII:
    /// - adopts or gets/creates a resource on construction
    /// - cleans up/releases a resource on destruction
    ///
    /// must supply appropriate callables for get and
    /// cleanup of the managed resource
    template <typename T>
    class RAII
    {
    public:
        using CleanupF = std::function<void(T)>;
        
        /// create the managed resource and also cleanup
        template <
            typename CreateF,
            typename = std::enable_if_t<
                std::is_invocable_r_v<T, CreateF>>
        >
        RAII( CreateF create, CleanupF cleanup )
            : _managed( create() )
            , _cleanup( cleanup )
        {}

        /// adopt an already existing resource and supply
        /// a cleanup function
        RAII( T __managed, CleanupF cleanup )
            : _managed( __managed )
            , _cleanup( cleanup )
        {}

        ~RAII()
        {
            _cleanup(_managed);
        }

        T managed() const { return _managed; }

    private:
        T _managed;
        CleanupF _cleanup;
    };


}
