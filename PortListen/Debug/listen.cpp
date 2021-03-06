/* listen.cpp */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include "listen.h"
#include <string.h>
#include <iostream>

int bindSock(uint16_t port)
{
	struct sockaddr_in sa;
	int sock;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1) {
		return sock;
	}

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
		close(sock);
		return -1;
	}

	if (listen(sock, SOMAXCONN) == -1) {
		close(sock);
		return -1;
	}

	return sock;
}

int main(int argc, char *argv[])
{
	int listen, conn;

	listen = bindSock(4444);
	if (listen == -1) {
		fprintf(stderr, "bindSock error\n");
		return 1;
	}
	char* recvData = new char[1024];
	int bytes;

	while ((conn = accept(listen, NULL, NULL)) > 0) {

			bytes = recv(conn, recvData, 1023, 0);
			if (bytes == -1) {
				std::cout << "error on recv" << std::endl;
			}
			else {
				recvData[bytes] = '\0';
				std::cout << recvData << std::endl;
			}

		close(conn);
	}

	close(listen);
	return 1;
}

