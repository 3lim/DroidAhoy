#hello_world: 1.1.cpp
#	g++ -o hello 1.1.cpp

GLFW = -lglfw3 -lGL -lGLU -lGLEW -lstdc++ -lm -lX11 -lXxf86vm -lXrandr -lpthread -lXi 

all: run_demo

run_demo: demo.cpp
	gcc -std=c++0x -Wall demo.cpp -o run_demo $(GLFW)

ahoy: ahoy_engine.cpp engine.o sm.o
	gcc -std=c++0x -Wall ahoy_engine.cpp engine.o sm.o -o run_ahoy $(GLFW)

engine.o: engine.cpp
	gcc -std=c++0x -Wall -c engine.cpp -o engine.o $(GLFW)

sm.o: shader_manager.cpp
	gcc -std=c++0x -Wall -c shader_manager.cpp -o sm.o $(GLFW)

%.out:	%.cpp
	gcc -std=c++0x -Wall $*.cpp -o $*.out $(GLFW)

clean: 
	-rm -rf *.o
	-rm run_demo
	-rm run_ahoy
