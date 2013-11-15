#hello_world: 1.1.cpp
#	g++ -o hello 1.1.cpp

BOOST = -lboost_system -lboost_system-mt -lboost_filesystem -lboost_filesystem-mt
GLFW =  -lglfw3 -lGL -lGLU -lGLEW -lstdc++ -lm -lX11 -lXxf86vm -lXrandr -lpthread -lXi $(BOOST) 

all: demo

demo: ahoy_engine.cpp engine.o shader_manager.o controls.o model_loader.o
	gcc -std=c++0x -Wall ahoy_engine.cpp engine.o shader_manager.o -o run_demo $(GLFW)

ahoy: ahoy_engine.cpp engine.o shader_manager.o controls.o model_loader.o particle.o simulation.o
	g++ -std=c++0x -Wall ahoy_engine.cpp engine.o shader_manager.o particle.o simulation.o -o run_ahoy $(GLFW)

%.o:	%.cpp
	gcc -std=c++0x -Wall -c $*.cpp -o $*.o $(GLFW)

%.out:	%.cpp
	gcc -std=c++0x -Wall $*.cpp -o $*.out $(GLFW)

clean: 
	-rm -rf *.o
	-rm -f run_demo
	-rm -f run_ahoy
