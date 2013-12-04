
/* PortListener.cpp */

#include "headers/port_listener.h"

using namespace glm;

glm::vec3 PortListener::orientation(0,0,0);

PortListener::PortListener(){
	//receiveData();
	//	pthread_create( &t1, NULL,&receiveData, NULL); // create a thread running recievedData
}

PortListener::~PortListener(){}

int PortListener::bindSock(uint16_t port)
{
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == -1) {
		return sock;
	}

	struct sockaddr_in sa;

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

void PortListener::receiveData(){

	char* recvData = new char[1024];
	int bytes;

	boundsocket = bindSock(4444);

	if (boundsocket == -1) {
    std::cerr << std::endl <<  "Could not bind the socket." << std::endl;
	}

	while ((conn = accept(boundsocket, NULL, NULL)) > 0) {

		bytes = recv(conn, recvData, 1023, 0);

		if (bytes == -1) {
      std::cerr << std::endl <<  "Error receiving data." << std::endl;
		}
		else {
      new_data = true;
			recvData[bytes] = '\0';
			orientation = charToFloat(recvData);

			std::cout<< "Received data:" <<glm::to_string(orientation)<<std::endl;

			oldVector = orientation;
		}

		close(conn);
	}
  std::cout << "Stopped listening for sockets." << std::endl;
	close(boundsocket);

}

glm::vec3 PortListener::charToFloat(char * characters){
	std::string tmp_s = characters;
	float * tmp_vf = new float[3];
	int place = 0;

	std::stringstream  ss;

	ss  << tmp_s;

	while( ! ss.eof() )
	{
		float  tmp_f;

		if ( ss >> tmp_f )
		{
			tmp_vf[place] = tmp_f;
			place++;
		}

	}

	//std::cout << tmp_vf[0] << tmp_vf[1] << tmp_vf[2] << std::endl;

	vec3 v(tmp_vf[0],tmp_vf[1],tmp_vf[2]);

	return v;
}
/*
void PortListener::startThread(){
    pthread_create(&thread, NULL, PortListener::receiveData, this);
}
 */

glm::vec3 PortListener::getVec(){
  if(new_data)
	  return orientation;
  else 
    return glm::vec3(0,0,0);
}
/*
void createThread(void){
	pthread_t t1;
	PortListener tmp;
	pthread_create( &t1, NULL, &PortListener::receiveData, NULL); // create a thread running recievedData
}
 */


//int main(int argc, char *argv[])
//{
//
//	//createThread();
//  std::thread receive(&PortListener::receiveData, PortListener());
//  receive.join();
//	//PortListener temp;
//
//	//while(true){
//	//	vec3 vertices = temp.getVec();
//	//	std::cout<< "Outside:" <<glm::to_string(vertices)<<std::endl;
//
//	//	sleep(1);
//	//}
//
//	//return 1;
//}

/*
void *handle(void *pnewsock)
{
     send(), recv(), close()

    return NULL;
}

int main(void)
{
    int sock;
    pthread_t thread;
    struct addrinfo hints, *res;
    int reuseaddr = 1;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }


    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }


    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }


    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 0;
    }

    freeaddrinfo(res);

    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        return 0;
    }

    while (true) {
        size_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
        if (newsock == -1) {
            perror("accept");
        }
        else {
            printf("Got a connection from %s on port %d\n",
                    inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
            if (pthread_create(&thread, NULL, handle, &newsock) != 0) {
                fprintf(stderr, "Failed to create thread\n");
            }
        }
    }

    close(sock);

    return 0;
}
 */
