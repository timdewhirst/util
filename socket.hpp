
#pragma once

#include <string>

class socket
{
public:
    virtual ~socket() = default;

    // blocking read; all data returned as
    // a string
    virtual std::string read() = 0;

    // blocking write
    virtual bool write(const std::string&) = 0;
};
