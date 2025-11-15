#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>
#include <string>

#define MAX_BUFF 1024
#include "src/question.hpp"

using namespace std;

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

Question parse(char* buffer) {
    cout << "Buf: " << buffer << endl;;
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
	int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 1) {cerr << "Error allocating socket"; exit(errno);}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		exit(errno);

	const char *msg = "Basic client-to-server test";
	send(sock, msg, strlen(msg), 0);

	char buffer[MAX_BUFF] = { 0 };
	recv(sock, buffer, MAX_BUFF, 0);
	cout << buffer << endl;
        
        memset(buffer, 0, MAX_BUFF); //get question
        recv(sock, buffer, MAX_BUFF, 0);
        //parse(buffer);
        string ans = to_string(ask(parse(buffer)));
        send(sock, ans.c_str(), strlen(ans.c_str()), 0);
        
        memset(buffer, 0, MAX_BUFF);
        recv(sock, buffer, MAX_BUFF, 0); //get correct flag
        if (buffer[0] - '0') {
            cout << "Correct Answer!" << endl;
        } else {
            cout << "Incorrect Answer!" << endl;
        }

	close(sock);
	return 0;
}
