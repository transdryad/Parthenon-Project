#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFF 1024
#include "src/question.hpp"

int ask(Question question) {
    std::cout << question.question << std::endl;
    std::cout << "1: " << question.answers[0] << std::endl;
    std::cout << "2: " << question.answers[1] << std::endl;
    std::cout << "3: " << question.answers[2] << std::endl;
    std::cout << "4: " << question.answers[3] << std::endl;
    std::cout << "Your Answer: ";
    int answer;
    std::cin >> answer;
    std::cout << std::endl;
    return answer;
}

using namespace std;

int
main(int argc, char **argv)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		return errno;

	const char *msg = "Basic client-to-server test";
	send(sock, msg, strlen(msg), 0);

	char buffer[MAX_BUFF] = { 0 };
	recv(sock, buffer, MAX_BUFF, 0);
	cout << buffer << endl;

	close(sock);
	return 0;
}
