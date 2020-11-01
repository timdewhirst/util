
#pragma once

#include <string>

/// construct a string matching the output of
/// hexdump -C:
///
/// 00000000  0a 23 70 72 61 67 6d 61  20 6f 6e 63 65 0a 0a 23  |.#pragma once..#|
/// 00000010  69 6e 63 6c 75 64 65 20  3c 66 75 6e 63 74 69 6f  |include <functio|
/// 00000020  6e 61 6c 3e 0a 23 69 6e  63 6c 75 64 65 20 3c 6d  |nal>.#include <m|
/// 00000030  65 6d 6f 72 79 3e 0a 0a  23 69 6e 63 6c 75 64 65  |emory>..#include|
/// 00000040  20 22 74 63 70 5f 73 6f  63 6b 65 74 2e 68 70 70  | "tcp_socket.hpp|
std::string hexdump(uint8_t* data, size_t len);
