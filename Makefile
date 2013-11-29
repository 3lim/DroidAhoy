#hello_world: 1.1.cpp
#	g++ -o hello 1.1.cpp

FILEMANAGEMENT = -lboost_system -lboost_filesystem -lSOIL
GLFW =  -lglfw3 -lGL -lGLU -lGLEW -lstdc++ -lm -lX11 -lXxf86vm -lXrandr -lpthread -lXi $(FILEMANAGEMENT) 
# GLFW =  -lglfw3 -lOpenGL32 -lGLU -lGLEW -lboost_filesystem -lboost_system -lSOIL

all: demo

demo: demo_engine.cpp engine.o shader_manager.o obj_loader.o model.o model_loader.o keyboard_controller.o controller.o tiny.o sph_simulation.o particle.o simulation_parameters.o spatial_hashing.o ocean_surface.o
	gcc -std=c++0x -Wall demo_engine.cpp engine.o keyboard_controller.o controller.o shader_manager.o model_loader.o model.o obj_loader.o tiny.o sph_simulation.o particle.o simulation_parameters.o spatial_hashing.o ocean_surface.o -o run_demo $(GLFW)

ahoy: ahoy_engine.cpp engine.o shader_manager.o obj_loader.o model.o model_loader.o keyboard_controller.o controller.o tiny.o sph_simulation.o particle.o simulation_parameters.o spatial_hashing.o ocean_surface.o
	gcc -std=c++0x -Wall ahoy_engine.cpp engine.o keyboard_controller.o controller.o shader_manager.o model_loader.o model.o obj_loader.o tiny.o sph_simulation.o particle.o simulation_parameters.o spatial_hashing.o ocean_surface.o -o run_ahoy $(GLFW)

# Compile all classes
%.o:	%.cpp
	g++ -std=c++0x -Wall -c $*.cpp -o $*.o $(GLFW)

# External lib
# tiny_obj_loader for .obj files
tiny.o: libs/tinyobj/tiny_obj_loader.cc
	gcc -std=c++0x -Wall -c libs/tinyobj/tiny_obj_loader.cc -o tiny.o $(GLFW)

clean: 
	-rm -rf *.o
	-rm -f run_demo
	-rm -f run_ahoy
