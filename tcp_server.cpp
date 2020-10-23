
#include "tcp_server.hpp"

// std
#include <cstring>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <atomic>
#include <iostream>
#include <thread>
#include <type_traits>
#include <stdexcept>

// local
#include "exception_builder.hpp"

template <typename T>
inline auto initialize(T& t) -> std::enable_if_t<std::is_standard_layout_v<T>>
{
    memset(&t, 0, sizeof(T));
}

template <typename T>
inline void typed_setsockopt(int fd, int level, int optname, T t)
{
    if (setsockopt(fd, level, optname, (const void*)&t, sizeof(T)) < 0)
        exception_builder<std::runtime_error>() << "failed to set socket option: " << strerror(errno);
}

class tcp_server::impl
{
public:
    callback_t callback;
    std::thread thread;
    std::atomic_bool should_stop = false;
    int socket_fd = -1;

    bool start(uint16_t port)
    {
        // create a passive socket, and place the accept into a thread
        // create an IPv4 TCP socket
        int sfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sfd < 0)
            exception_builder<std::runtime_error>() << "failed to create socket: " << strerror(errno);

        // set option reuse address to ensure server can restart quickly
        typed_setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, 1);

        // bind the socket to an address
        sockaddr_in server_addr;
        initialize(server_addr);

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
            exception_builder<std::runtime_error>() << "failed to bind: " << strerror(errno);

        // listen, maximum of five backlogged connections
        listen(sfd, 5);

        // okay we're ready to go! spawn a thread to accept incoming connections
        // and call our callback (if present).
        // we're going to use select to figure out if there is any activity
        // on the server socket; if there is then we can accept() knowing that
        // the call to accept() won't block
        socket_fd = sfd;
        thread = std::thread(
            [this]()
            {
                fd_set read_set;
                FD_ZERO(&read_set);

                while (!should_stop)
                {
                    FD_SET(socket_fd, &read_set);
                    timeval timeout{ 0, 10000 }; // 10ms
                    select(socket_fd + 1, &read_set, nullptr, nullptr, &timeout);

                    if (FD_ISSET(socket_fd, &read_set))
                    {
                        sockaddr_in client_addr;
                        socklen_t len = sizeof(client_addr);
                        int fd = accept(socket_fd, (sockaddr*)&client_addr, &len);
                        if (fd<0)
                            exception_builder<std::runtime_error>() << "failed to accept: " << strerror(errno);

                        // okay, got a client connection, let's dump some info and see if we have a callback
                        std::cout << "got a connection!\n";
                        tcp_socket client(fd);
                        if (callback)
                        {
                            callback(std::move(client));
                        }
                        else
                        {
                            // write a response and close; will be closed when client goes out of scope
                            client.write("no callback; goodbye!\n");
                        }
                    }
                }
            });

        return true;
    }

    bool stop()
    {
        // thread isn't running
        if (!thread.joinable())
            return false;

        // already signalled to stop
        if (should_stop)
            return false;

        should_stop = true;
        thread.join();
        return true;
    }
};

tcp_server::tcp_server()
    : _impl(new impl)
{}

tcp_server::~tcp_server() = default;

bool tcp_server::start(uint16_t port)
{
    return _impl->start(port);
}

bool tcp_server::stop()
{
    return _impl->stop();
}

bool tcp_server::is_running() const
{
    return _impl->thread.joinable();
}

void tcp_server::set_callback(callback_t callback)
{
    _impl->callback = callback;
}
