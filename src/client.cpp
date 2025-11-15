#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFF 1024

using namespace std;

int
main(int argc, char **argv)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080); /* change in prod */
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(sock, (struct sockaddr*)&addr, sizeof(addr));

	const char *msg = "Basic client-to-server test";
	send(sock, msg, strlen(msg), 0);

	char buffer[MAX_BUFF] = { 0 };
	recv(sock, buffer, MAX_BUFF, 0);
	cout << buffer << endl;

	close(sock);

    return 0;
}
