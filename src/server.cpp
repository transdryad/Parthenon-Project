#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>

#define MAX_QUEUE 5
#define MAX_BUFF 1024
#include <vector>
#include <optional>
#include "src/question.hpp"
#include "src/toml.hpp"

std::vector<Question> parse(std::string filename) {
    toml::table tbl;
    std::vector<Question> questions;

    try { //parse toml into memory
        tbl = toml::parse_file(filename);
    } catch (const toml::parse_error& err) {
        std::cerr << "Parsing failed:\n" << err << "\n";
        exit(1);
    }

    //std::cout << tbl["questions"] << std::endl;
    for (size_t i = 0; i < tbl["questions"].as_array()->size(); i++) { //put questions in our nice data structure
        std::optional<std::string> question = tbl["questions"][i][0].value<std::string>();
        std::optional<int> correct = tbl["questions"][i][2].value<int>();

        std::string answers[4];
        for (int j = 0; j < 4; j++) {
            std::optional<std::string> thing = tbl["questions"][i][1][j].value<std::string>();
            answers[j] = thing.value_or("");
        }

        questions.emplace_back(question.value_or(""), answers, correct.value_or(0));
    }

    return questions;
}

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <ip> <addr> <qfile>" << std::endl;
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 1) {cerr << "Error allocating socket"; exit(errno);}
    int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int b = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (b == -1) {cerr << "Error binding to port"; exit(errno);}
    listen(sock, MAX_QUEUE); /* should probably paralellized,
                                so small queue per instance, if such */

    int csock = accept(sock, nullptr, nullptr);
    if (csock < 1) {cerr << "Error allocating socket"; exit(errno);}
    char buffer[MAX_BUFF] = { 0 };
    recv(csock, buffer, MAX_BUFF, 0);
    cout << buffer << endl;

    const char *msg = "Basic server-to-client test";
    send(csock, msg, strlen(msg), 0);

    std::vector<Question> questions = parse(argv[3]);

    for (auto q : questions) {
        string packet;
        packet = q.question + "%" + q.answers[0] + "%" + q.answers[1] + "%" + q.answers[2] + "%" + q.answers[3];
        cout << packet << endl;
        send(csock, packet.c_str(), strlen(packet.c_str()), 0);
        memset(buffer, 0, MAX_BUFF);
        recv(csock, buffer, MAX_BUFF, 0);
        int ans = buffer[0] - '0';
        cout << ans << endl;
        if (q.correct == ans) { //TODO: score tracking here
            send(csock, "1", 1, 0);
        } else {
            send(csock, "0", 1, 0);
        }
        //std::cout << q.question << ":";
        //for (auto a : q.answers) { std::cout << " " << a; }
        //std::cout << ". " << q.correct << std::endl;
    }
    close(csock);
    close(sock);

    return 0;
}
