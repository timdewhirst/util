
#include "tcp_socket.hpp"

// std
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <array>
#include <iostream>
#include <stdexcept>

// local
#include "exception_builder.hpp"

// impl
class tcp_socket::impl
{
public:
    int socket_fd = -1;

    impl(int sfd)
        : socket_fd(sfd)
    {}

    impl(const std::string& address, uint16_t port)
    {
        // setup hints
        addrinfo hints;
        memset(&hints, 0, sizeof(addrinfo));
        hints.ai_family = AF_UNSPEC;     // IPv4 or v6
        hints.ai_socktype = SOCK_STREAM; // TCP
        hints.ai_flags = 0;
        hints.ai_protocol = 0;           // any protocol

        // get possible resolved addresses
        addrinfo* result;
        int s = getaddrinfo(address.c_str(), std::to_string(port).c_str(),
                            &hints,
                            &result);
        if (s != 0)
            exception_builder<std::runtime_error>() << "getaddrinfo failed: " << gai_strerror(s);

        // iterate over results until we successfully open a socket and connect
        for ( auto* r = result; r != nullptr; r = r->ai_next )
        {
            int sfd = ::socket(r->ai_family, r->ai_socktype, r->ai_protocol);
            if (sfd == -1)
                continue;

            if (connect(sfd, r->ai_addr, r->ai_addrlen) != -1)
            {
                // success; stash sfd
                socket_fd = sfd;
                break;
            }

            close(sfd);
        }

        if (!result)
            exception_builder<std::runtime_error>() << "failed to resolve address for: "
                                                    << address << ":" << port;
        // clean up the result from getaddrinfo
        freeaddrinfo(result);
    }

    ~impl()
    {
        if (socket_fd != -1)
        {
            ::close(socket_fd);
            socket_fd = -1;
        }
    }
};

tcp_socket::tcp_socket(int socket_fd)
    : _impl(new impl(socket_fd))
{}

tcp_socket::tcp_socket(const std::string& address, uint16_t port)
    : _impl(new impl(address, port))
{}

tcp_socket::~tcp_socket() = default;

tcp_socket::tcp_socket(tcp_socket&&) = default;
tcp_socket& tcp_socket::operator=(tcp_socket&&) = default;

std::string tcp_socket::read()
{
    std::string result;
    std::array<char, 1024> buffer = {};

    while (true)
    {
        int bytes_read = ::read(_impl->socket_fd, buffer.data(), buffer.size());
        if (bytes_read<0)
            exception_builder<std::runtime_error>() << "failed to read: " << strerror(errno);

        result += buffer.data();
        if (bytes_read < (int)buffer.size())
            break;
    }

    return result;
}

bool tcp_socket::write(const std::string& s)
{
    if ( _impl->socket_fd == -1 )
        return false;

    auto written = ::send(_impl->socket_fd, s.c_str(), s.size(), MSG_NOSIGNAL);
    if (written < 0)
        return false;

    if (written != (int)s.size())
        return false;

    return true;
}

