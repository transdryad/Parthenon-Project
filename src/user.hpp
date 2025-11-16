#pragma once

#include <string>

class User {
    public:
        std::string name;
        int sock;
        int score = 0;
        User(int sock);
};
