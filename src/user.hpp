#pragma once

#include <string>
#include <unistd.h>
#include <cstdint>

class User {
    public:
        std::string name;
        int sock;
        uint32_t score = 0;
        User(int sock);
};
