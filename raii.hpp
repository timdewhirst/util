
#pragma once

// std
#include <functional>
#include <type_traits>

namespace util {

    /// a generalized wrapper for RAII managing ownership/cleanup
    /// of an instance of \tparam T t:
    /// - adopts a resource on construction
    /// - cleans up the resource on destruction
    ///
    /// must supply appropriate callable for
    /// cleanup of the managed resource
    template <
        typename T,
        typename = std::enable_if_t<std::is_pointer_v<T>>
    >
    class RAII
    {
    public:
        using CleanupF = std::function<void(T)>;

        /// adopt an already existing resource and supply
        /// a cleanup function
        RAII( T __managed, CleanupF cleanup )
            : _managed( __managed )
            , _cleanup( cleanup )
        {}

        /// cannot copy
        RAII(const RAII&) = delete;
        RAII& operator=(const RAII&) = delete;

        // can move
        RAII(RAII&& m)
        {
            _managed = m._managed;
            _cleanup = m._cleanup;

            m._managed = nullptr;
            m._cleanup = {};
        }

        RAII& operator=(RAII&& m)
        {
            _managed = m._managed;
            _cleanup = m._cleanup;

            m._managed = nullptr;
            m._cleanup = {};

            return *this;
        }

        ~RAII()
        {
            if ( _cleanup )
                _cleanup(_managed);
        }

        const T& managed() const { return _managed; }
        T& managed() { return _managed; }

    private:
        mutable T _managed;
        CleanupF _cleanup;
    };


}
