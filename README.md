# Utils

A set of (hopefully) handy snippets in C++/STL.

## TCP client/server

A set of classes to support simple TCP client/server communication (unix only); deliberately constructed to use blocking/threading instead of a concurrent, singe threaded, event driven approach.

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

The two classes allow for simple client/server communication e.g. a trivial client:

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

### Running

To run the server:

* ./server 9999

You can then telnet to the server; the example by default will echo back your input:

```sh
> telnet localhost 9999
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
hello, world!
you wrote: hello, world!
quit
Connection closed by foreign host.
```

The server will show the following:

```sh
> ./server 9999
started server on port: 9999...
got a connection!
starting client: 139974019290880
client shutdown: 139974019290880
```