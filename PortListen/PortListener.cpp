/* listen.cpp */

#include "PortListener.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>

using namespace glm;

PortListener::~PortListener(){}

int PortListener::bindSock(uint16_t port)
{
	struct sockaddr_in sa;

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

glm::vec3 PortListener::receiveData(){

	char* recvData = new char[1024];
	int bytes;

	listen = bindSock(4444);

	if (listen == -1) {
		fprintf(stderr, "bindSock error\n");
	}

	while ((conn = accept(listen, NULL, NULL)) > 0) {

		bytes = recv(conn, recvData, 1023, 0);
		if (bytes == -1) {
			std::cout << "error on recv" << std::endl;
		}
		else {

			float x = (float)recvData[0];
			float y = (float)recvData[1];
			float z = (float)recvData[2];

			return {x,y,z};

			//std::cout << x << y << z << std::endl;


			//recvData[bytes] = '\0';
			//std::cout << recvData << std::endl;


		}

		close(conn);
	}

	close(listen);

	return (glm::vec3 v(0,0,0));
}

int main(int argc, char *argv[])
{
	//receiveData();
	return 1;
}

