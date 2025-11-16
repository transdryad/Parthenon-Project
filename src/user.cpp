#include "src/user.hpp"

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

User::User(int sock) {
    char buffer[1024] = { 0 };
    uint16_t name_len;
    recv(sock, &name_len, 2, 0);
    name_len = ntohs(name_len);
    recv(sock, buffer, name_len, 0);
    this->name = buffer;
}
