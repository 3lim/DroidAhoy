//#include "Controls.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>


class PortListener /*: public Controls */{
private:
	PortListener();

	int listen, conn;

	int sock;

public:
	virtual ~PortListener();

	glm::vec3 receiveData();
	int bindSock(uint16_t port);


};
