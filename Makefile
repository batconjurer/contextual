
CFLAGS=-Wfatal-errors -DNDEBUG -march=native -ffast-math -std=c++17 -O0 -fPIC -Iinclude
LDFLAGS=


all: example

example: tests/example.cpp
	g++ -o example $(CFLAGS) example.cpp $(LDFLAGS) 
