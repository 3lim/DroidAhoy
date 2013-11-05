#hello_world: 1.1.cpp
#	g++ -o hello 1.1.cpp

GLFW = -lglfw3 -lGL -lGLU -lstdc++ -lm -lX11 -lXxf86vm -lXrandr -lpthread -lXi 

all: run_demo

run_demo: demo.cpp
	gcc -std=c++0x -Wall demo.cpp -o run_demo $(GLFW)

%.out:	%.cpp
	gcc -std=c++0x -Wall $*.cpp -o $*.out $(GLFW)

clean: 
	-rm -rf *.o
	-rm run_demo
