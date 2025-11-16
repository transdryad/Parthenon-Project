#pragma once

#include <string>
#include <unistd.h>

class User {
    public:
        std::string name;
        int sock;
        uint32_t score = 0;
        User(int sock);
};
