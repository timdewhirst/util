
// std
#include <iostream>
#include <string>

// local
#include "uuid.hpp"

int main(int argc, char* argv[])
{
    std::cout << util::uuid{} << "\n";
    std::cout << util::uuid::create_v4() << "\n";

    return 0;
}
