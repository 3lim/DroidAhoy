//#include "Controls.h"

//Socket libraries
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <string.h>
#include <iostream>
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
//print vec
#include "glm/ext.hpp"
//read in string
#include <sstream>
//threads
#include <pthread.h>
#include <thread>

#define PORT "4444" /* Port to listen on */
#define BACKLOG 10 /* Passed to listen() */

class PortListener {

public:

	PortListener();
	int bindSock(uint16_t port);

	//void startThread();
	//static void receiveData(void * c);
	void receiveData();

	~PortListener();
	glm::vec3 charToFloat(char * characters);
	glm::vec3 getVec();

	pthread_t t1; // declare thread

	int boundsocket;
	static glm::vec3 orientation;



private:
  bool new_data;
	int conn;
	int sock;
	glm::vec3 oldVector;

	pthread_t thread;
};

