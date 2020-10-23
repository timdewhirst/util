
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "tcp_server.hpp"
#include "tcp_socket.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " <port>\n";
        return 1;
    }

    try
    {
        tcp_server server;
        uint16_t port = std::stoi(argv[1]);
        server.set_callback(
            [](tcp_socket&& s)
            {
                // when we get a connection, spawn a new thread to handle
                // the connection. Be aware, the thread is not being cleaned up!!
                new std::thread(
                    [s = std::move(s)]() mutable
                    {
                        std::cout << "starting client: " << std::this_thread::get_id() << "\n";
                        while (true)
                        {
                            auto received = s.read();
                            if (strncmp(received.data(), "quit", 4) == 0)
                            {
                                std::cout << "client shutdown: " << std::this_thread::get_id() << "\n";
                                return;
                            }

                            if (!s.write("you wrote: " + received))
                            {
                                std::cout << "connection closed: " << std::this_thread::get_id() << "\n";
                                return;
                            }
                        }
                    });
            });
        server.start(port);
        std::cout << "started server on port: " << port << "...\n";

        // check for local termination of the server
        std::string repl;
        while (true)
        {
            std::getline(std::cin, repl);
            if (repl == "quit")
                break;
        }

        std::cout << "server shutting down...";
        server.stop();
        std::cout << "done!\n";
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
