#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_QUEUE 5
#define MAX_BUFF 1024

using namespace std;

int main(int argc, char **argv)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	bind(sock, (struct sockaddr*)&addr, sizeof(addr));

	listen(sock, MAX_QUEUE); /* should probably paralellized,
	so small queue per instance, if such */

	int csock = accept(sock, nullptr, nullptr);

	char buffer[MAX_BUFF] = { 0 };
	recv(csock, buffer, MAX_BUFF, 0);
	cout << buffer << endl;

	const char *msg = "Basic server-to-client test";
	send(csock, msg, strlen(msg), 0);

	close(sock);

	return 0;
}
