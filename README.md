# Utils

A set of (hopefully) handy snippets in C++/STL.

## TCP client/server

A set of classes to support simple TCP client/server communication (unix only):

**tcp_socket** is an owning wrapper around a file handle; tcp_socket can be created either with an already opened file handler,
or with a hostname/port pair. Any issues during setup will throw an exception containing the stringified errno.

**tcp_server** is slightly more complex, and will:

* create a socket
* bind the socket to a port
* listen for connections
* spawn a thread which will
  * loop until stopped
  * check for activity on the socket
  * when activity is seen, calls accept
  * if a callback has been registered: invokes the callback with an rvalue reference tcp_socket to the client
  * if a callback has not been registered: write a short message to the client, and closes the connection

The two classes allow for simple client/server communication:

```c++
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
```

### Building

* g++ -Wall -Werror -g -std=c++17 -o server server_main.cpp tcp_server.cpp tcp_socket.cpp -lpthread
* g++ -Wall -Werror -g -std=c++17 -o client client_main.cpp tcp_socket.cpp -lpthread

