
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
