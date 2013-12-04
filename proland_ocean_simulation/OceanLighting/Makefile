CC = g++
CFLAGS = -std=c++0x
LIBS = -L/usr/lib -lGLEW -lGL -lGLU -lglut -lstdc++ -lm  -lAntTweakBar
INCS = -I/usr/include 
all: main

main: Main.o Program.o 
	$(CC) $(INCS) $(CFLAGS) *.o -o program $(LIBS)

Main.o: Main.cpp
	$(CC) $(INCS) $(CFLAGS) -c Main.cpp $(LIBS)

Program.o: Program.cpp
	$(CC) $(INCS) $(CFLAGS) -c Program.cpp $(LIBS)

clean: 
	-rm -rf *.o
	-rm -f program





