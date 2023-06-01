
#pragma once

#include <functional>
#include <memory>

#include "tcp_socket.hpp"

/// a simple TCP server:
/// - accepts all connections
/// - calls the registered callback when a new connection is accepted
/// - the callback will be passed an instance of tcp_socket
/// - the callback will happen in a thread context owned by server
class tcp_server
{
public:
    using callback_t = std::function<void(tcp_socket&&)>;

    tcp_server();
    ~tcp_server();

    /// start listening
    bool start(uint16_t port);
    bool stop();

    /// are we running?
    bool is_running() const;

    /// set callback to be invoked on client connections
    void set_callback(callback_t callback);

private:
    class impl;
    std::unique_ptr<impl> _impl;
};
