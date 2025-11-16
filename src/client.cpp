#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <string>
#include <poll.h>
#include <time.h>

#define MAX_BUFF 1024
#include "src/question.hpp"

using namespace std;

int ask(Question question, time_t *time_out) {
    std::cout << question.question << std::endl;
    std::cout << "1: " << question.answers[0] << std::endl;
    std::cout << "2: " << question.answers[1] << std::endl;
    std::cout << "3: " << question.answers[2] << std::endl;
    std::cout << "4: " << question.answers[3] << std::endl;
    std::cout << "Your Answer: " << std::endl;
    int answer = 0;
    struct pollfd pfd = {.fd = 0, .events = POLLIN, .revents = 0};
    poll(&pfd, 1, 30000);
    time_t time_start = time(nullptr);
    if((pfd.revents & POLLIN) != 0)
        std::cin >> answer;
    time_t time_end = time(nullptr);
    time_out[0] = time_end - time_start;
    std::cout << answer << std::endl;
    return answer;
}

Question parse(char* buffer) {
    // cout << "Buf: " << buffer << endl;;
    stringstream ss(string(buffer, strlen(buffer)));
    string t;
    string strings[5];
    int i = 0;
    while (getline(ss, t, '%')) {
        strings[i] = t;
        i++;
    }
    string answers[4] = {strings[1], strings[2], strings[3], strings[4]};
    return Question(strings[0], answers, 5);
}

int
main(int argc, char **argv)
{
    if(argc < 4){
        cerr << "Usage: " << argv[0] << " <ipv4 server> <port> <nickname>" << endl;
        exit(-1);
    }

	int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 1) {
        cerr << "Error allocating socket" << endl;
        exit(errno);
    }
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		exit(errno);

	char buffer[MAX_BUFF] = { 0 };
	recv(sock, buffer, MAX_BUFF, 0);
	cout << buffer << endl;

    uint16_t nick_len = strlen(argv[3]);
    nick_len = htons(nick_len);
	if(send(sock, &nick_len, 2, 0) != 2)
	    cerr << "nick_len not fully written!" << endl;
	if(send(sock, argv[3], nick_len, 0) < nick_len)
	    cerr << "argv[3] not fully written!" << endl;

    while(1){    
        memset(buffer, 0, MAX_BUFF);
        //struct pollfd pfd = {.fd = sock, .events = POLLIN, .revents = 0};
        //while((pfd.revents & POLLIN) == 0)
        //    poll(&pfd, 1, -1);
        recv(sock, buffer, MAX_BUFF, 0);
        if(buffer[0] == '\xFF')
            break;
        //parse(buffer);
        time_t timer = 0;
        uint8_t ans = ask(parse(buffer), &timer);
        //std::cout << "htonl(ask(parse(buffer))) = " << (int)ans << std::endl;
        send(sock, &ans, 1, 0);
        uint32_t timepkt = htonl(timer);
        send(sock, &timepkt, 4, 0);
    }

    uint32_t score = 0;
    recv(sock, &score, 4, 0);
    // cout << "unconverted score: " << score << endl;
    score = ntohl(score);

    cout << "Score: " << score << endl;

	close(sock);
	return 0;
}
