
#include <string>
#include <iostream>

#include "tcp_socket.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "usage: " << argv[0] << " <address> <port>\n";
        return 1;
    }

    try
    {
        tcp_socket connection(argv[1], std::stoi(argv[2]));
        connection.write("hello, world!");
        std::cout << connection.read();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
