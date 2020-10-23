
#pragma once

// std
#include <memory>

// local
#include "socket.hpp"

/// thin owning wrapper around a file descriptor; will close the
/// file when tcp_socket is destroyed. tcp_socket is moveable but
/// not copyable
class tcp_socket : public socket
{
public:
    tcp_socket(int socket_fd);
    tcp_socket(const std::string& address, uint16_t port);
    tcp_socket(const tcp_socket&) = delete;
    tcp_socket(tcp_socket&&);
    ~tcp_socket() override;

    tcp_socket& operator=(const tcp_socket&) = delete;
    tcp_socket& operator=(tcp_socket&&);

    /// overrides from client_socket
    std::string read() override;
    bool write(const std::string&) override;

private:
    class impl;
    std::unique_ptr<impl> _impl;
};
