FILEMANAGEMENT = -lboost_system -lboost_filesystem -lSOIL
GLFW =  -ggdb -lglfw3 -lGL -lGLU -lGLEW -lstdc++ -lm -lX11 -lXxf86vm -lXrandr -lpthread -lXi $(FILEMANAGEMENT) 
# GLFW =  -lglfw3 -lOpenGL32 -lGLU -lGLEW -lboost_filesystem -lboost_system -lSOIL

all: ahoy

ahoy: ahoy_engine.cpp port_listener.o android_controller.o engine.o shader_manager.o obj_loader.o transformable.o model.o model_loader.o keyboard_controller.o controller.o tiny.o sph_simulation.o particle.o simulation_parameters.o spatial_hashing.o ocean_surface.o camera.o boat.o
	g++ -std=c++0x -Wall ahoy_engine.cpp port_listener.o android_controller.o engine.o keyboard_controller.o controller.o shader_manager.o model_loader.o transformable.o model.o obj_loader.o tiny.o sph_simulation.o particle.o simulation_parameters.o spatial_hashing.o camera.o boat.o ocean_surface.o -o run_ahoy $(GLFW)

# Compile all classes
%.o:	%.cpp
	g++ -std=c++0x -Wall -c $*.cpp -o $*.o $(GLFW)

# External lib
# tiny_obj_loader for .obj files
tiny.o: libs/tinyobj/tiny_obj_loader.cc
	g++ -std=c++0x -Wall -c libs/tinyobj/tiny_obj_loader.cc -o tiny.o $(GLFW)

clean: 
	-rm -rf *.o
	-rm -f run_demo
	-rm -f run_ahoy
