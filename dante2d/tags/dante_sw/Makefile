# Makefile for Dante 2D
# *****************************************************
# Variables to control Makefile operation
CXX = g++
CXXFLAGS = -Wall -g
SDL_FLAGS = `sdl-config --cflags` `sdl-config --libs` -lSDL_image
# ****************************************************
# Targets needed to bring the executable up to date

test: test.o event.o sprite.o control.o engine.o player.o
	$(CXX) $(CXXFLAGS) $(SDL_FLAGS) -o test test.o event.o sprite.o control.o engine.o player.o

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp

dante: main.o event.o sprite.o control.o engine.o player.o
	$(CXX) $(CXXFLAGS) $(SDL_FLAGS) -o dante main.o event.o sprite.o control.o engine.o player.o

main.o: main.cpp event.h
	$(CXX) $(CXXFLAGS) -c main.cpp

event.o: event.cpp event.h
	$(CXX) $(CXXFLAGS) -c event.cpp

sprite.o: sprite.cpp sprite.h
	$(CXX) $(CXXFLAGS) -c sprite.cpp

control.o: control.cpp control.h
	$(CXX) $(CXXFLAGS) -c control.cpp

engine.o: engine.cpp engine.h
	$(CXX) $(CXXFLAGS) -c engine.cpp

player.o: player.cpp player.h
	$(CXX) $(CXXFLAGS) -c player.cpp
clean:
	rm *.o dante test