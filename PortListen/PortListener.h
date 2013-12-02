#include "Controls.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>


class PortListen : public Controls {
private:
	PortListen();

	int sock;
	int listen, conn;
	char* recvData = new char[1024];
	int bytes;

public:
	~PortListen();

	receiveData();

	glm::vec3 <float> receiveData();

	virtual ~listen();
};

/*
 #include "engine.h"

class AhoyEngine : public Engine{
  private:
    AhoyEngine();

  public:
    ~AhoyEngine();
    AhoyEngine(int width, int height, string name) : Engine(width,height,name){
    }
    int init();
    int update();
    int render();

  protected:

};
 */
