#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <string>
#include <cstdio>
#include <poll.h>

#define MAX_QUEUE 5
#define MAX_BUFF 1024
#include <vector>
#include <optional>
#include <fstream>
#include "src/question.hpp"
#include "src/user.hpp"

std::string split(std::string input) {
    std::string substring;
    substring = input.substr(input.find("\"") + 1);
    substring.pop_back();
    return substring;
}

std::vector<Question> parse(std::string filename) {
    std::vector<Question> questions;
    std::vector<std::string> lines;
    std::ifstream ifs(filename);
    std::string line;

    if (ifs.is_open()) {
        while (getline(ifs, line)) {
            lines.push_back(line);
        }
    } else {
        std::cerr << "File note found/read error for questions";
        exit(1);
    }
    
    ifs.close();
    
    //std::cout << tbl["questions"] << std::endl;
    for (size_t i = 0; i < lines.size(); i++) { //put questions in our nice data structure
        std::string question;
        std::string answers[4];
        int correct;

        if (lines[i] == "[Question]") {
            ++i;
            question = split(lines[i]);
            ++i;
            answers[0] = split(lines[i]);
            ++i;
            answers[1] = split(lines[i]);
            ++i;
            answers[2] = split(lines[i]);
            ++i;
            answers[3] = split(lines[i]);
            ++i;
            correct = lines[i].back() - '0';
            questions.emplace_back(question, answers, correct);
        }
    }

    return questions;
}

int
send_question(User user, const void *buf, size_t size, int correct)
{
    std::cout << "User socket fd: " << user.sock << std::endl;
    send(user.sock, buf, size, 0);
    time_t time_start = time(nullptr);
    uint32_t ans = 0;

    struct pollfd pfd = {.fd = user.sock, .events = POLLIN, .revents = 0};
    while((pfd.revents & POLLIN) == 0)
        poll(&pfd, 1, -1);
    recv(user.sock, &ans, 4, 0);
    time_t time_end = time(nullptr);
    ans = ntohl(ans);
    std::cout << "Answered " << ans << std::endl;
    if(ans == correct){
        time_t loss = (1000 / 60) * (time_end - time_start);
        return 1000 - loss;
    } else
        return 0;
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port> <qfile>" << std::endl;
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 1) {std::cerr << "Error allocating socket"; exit(errno);}
    int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    int b = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (b == -1) {std::cerr << "Error binding to port"; exit(errno);}

    std::vector<User> users;

    std::cout << "Lobby open, press ENTER to start game." << std::endl;
    char ch = '\0';
    while(ch != '\n'){
        //std::cout << "Looped - listening again." << std::endl;
        listen(sock, MAX_QUEUE); /* should probably paralellized,
                                    so small queue per instance, if such */
        int csock = accept(sock, nullptr, nullptr);
        if(csock < 1){
            std::cerr << "Error allocating socket" << std::endl;
        }
        const char *msg = "Connected!";
        send(csock, msg, strlen(msg), 0);

        users.emplace_back(csock);
        std::cout << users.back().name << " joined." << std::endl;
        ch = std::getchar();
    }

    std::vector<Question> questions = parse(argv[3]);

    std::cout << "Number of questions: " << questions.size() << std::endl;
    std::cout << "Number of users: " << users.size() << std::endl;

    for (auto q : questions) {
        std::string packet;
        packet = q.question + "%" + q.answers[0] + "%" + q.answers[1] + "%" + q.answers[2] + "%" + q.answers[3];
        //cout << packet;

        for(auto u : users){
            std::cout << "Polling user " << u.name << " with question \"" << q.question << "\"." << std::endl;
            u.score += send_question(u, packet.c_str(), strlen(packet.c_str()), q.correct);
        }
        
        //std::cout << q.question << ":";
        //for (auto a : q.answers) { std::cout << " " << a; }
        //std::cout << ". " << q.correct << std::endl;
    }
    const char endmsg = '\xFF';
    for(auto u : users){
        send(u.sock, &endmsg, 1, 0);
        //std::cout << "Score: " << u.score << std::endl;
        u.score = htonl(u.score);
        send(u.sock, &u.score, 4, 0);
        close(u.sock);
    }

    close(sock);

    return 0;
}
