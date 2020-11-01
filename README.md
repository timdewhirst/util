# Utils

A set of (hopefully) handy snippets in C++/STL; designed to be as minimal as possible, and can be used by including the header and building the corresponding source (if required).

## simple_unit_test

A trivial and very lightweight alternative to gtest/doctest/... Designed to give the absolute bare minimum: an assertion with description on failure, and a summary:

```c++
// std
#include <cstring>

// local
#include "simple_unit_test.hpp"

namespace util::test {

    // custom equality check for char arrays with length 4
    bool equal(const char (&a)[4], const char (&b)[4])
    {
        return strncmp(a, b, 4) == 0;
    }

}

int main(int argc, char* argv[])
{
    ASSERT_EQUAL(1, 1);
    ASSERT_EQUAL(1, 2);
    ASSERT_EQUAL("foo!", "foo!"); // comparing pointers

    {
        const char a[] = "foo!";
        const char b[] = "foo!";
        ASSERT_EQUAL(a, b);       // note this failure; unexpected!
    }

    {
        const char a[] = "foo";
        const char b[] = "foo";
        ASSERT_EQUAL(a, b);       // this passes thanks to the custom equal function
    }

    return 0;
}
```

```sh
> ./a.out 
failure at line 21(a: actual, e: expected)
a: 1
e: 2
failure at line 27(a: actual, e: expected)
a: foo!
e: foo!
total tests: 5, passed: 3, failed: 2

```

The test is performed by calling `equal` for the two arguments; this function can be overloaded to support specific scenarios. The overloaded functions must be in the `util::test` namespace.

### Building

* g++ -g -Wall -Werror -std=c++17 simple_unit_test_main.cpp

## hexdump

Writes an array of data to a std::string, in the same format as `hexdump -C`:

```
> hexdump -C test.bin 
00000000  bf a0 98 88 e2 45 2b c0  81 ef aa 63 e6 42 f1 3e  |.....E+....c.B.>|
00000010  7c 31 69 33 e4 77 e9 f5  40 9a 36 8f 62 92 34 24  ||1i3.w..@.6.b.4$|
00000020  42 c0 f4 08 91 c8 14 a6  05 c0 9b b4 6a ca c3 4e  |B...........j..N|
00000030  15 49 a8 a7 7e 88 ef 02  78 5e 21 e8 90 9a 36 dc  |.I..~...x^!...6.|
00000040  ad 9f 05 9e ad 9d 95 f7  b6 bc 8f 5c 39 bf 80 92  |...........\9...|
00000050  18 3c 80 52 65 b4 ed 9c  d7 3a b0 4c 35 9a 70 86  |.<.Re....:.L5.p.|
00000060  27 0d 7b aa 15 f4 31 90  cb c6 bc b2 a2 d4 4c 68  |'.{...1.......Lh|
00000070  60 d8 ac e5 eb 42 cd 17  c0 0c 9e e4 2e cf 48 25  |`....B........H%|
00000080
> ./a.out < test.bin 
00000000  bf a0 98 88 e2 45 2b c0  81 ef aa 63 e6 42 f1 3e  |.....E+....c.B.>|
00000010  7c 31 69 33 e4 77 e9 f5  40 9a 36 8f 62 92 34 24  ||1i3.w..@.6.b.4$|
00000020  42 c0 f4 08 91 c8 14 a6  05 c0 9b b4 6a ca c3 4e  |B...........j..N|
00000030  15 49 a8 a7 7e 88 ef 02  78 5e 21 e8 90 9a 36 dc  |.I..~...x^!...6.|
00000040  ad 9f 05 9e ad 9d 95 f7  b6 bc 8f 5c 39 bf 80 92  |...........\9...|
00000050  18 3c 80 52 65 b4 ed 9c  d7 3a b0 4c 35 9a 70 86  |.<.Re....:.L5.p.|
00000060  27 0d 7b aa 15 f4 31 90  cb c6 bc b2 a2 d4 4c 68  |'.{...1.......Lh|
00000070  60 d8 ac e5 eb 42 cd 17  c0 0c 9e e4 2e cf 48 25  |`....B........H%|

```

```c++
// std
#include <iostream>
#include <string>

// local
#include "hexdump.hpp"

int main(int argc, char* argv[])
{
    std::string data_in;
    std::getline(std::cin, data_in);
    std::cout << hexdump((uint8_t*)data_in.data(), data_in.size());

    return 0;
}
```

### Building

* g++ -g -Wall -Werror -std=c++17 hexdump_main.cpp hexdump.cpp

### Testing

`hexdump` has unit testing built in, using the simple_unit_test framework. To build the unit tests:

* g++ -g -Wall -Werror -std=c++17 -DTEST_MAIN hexdump.cpp
* run the resulting binary
* result should be:

```
> ./a.out 
total tests: 23, passed: 23, failed: 0
```

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