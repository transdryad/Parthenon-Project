#include "src/user.hpp"

#include <string>
#include <sys/socket.h>

User::User(int sock) {
    char buffer[20] = { 0 };
    recv(sock, buffer, 20, 0);
    this->name = buffer;
}
