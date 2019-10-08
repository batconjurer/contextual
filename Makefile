
CFLAGS=-Wall -DNDEBUG -march=native -ffast-math -std=c++17 -O3 -fPIC -Iinclude
LDFLAGS=


all: example

example: tests/example.cpp
	g++ -o example $(CFLAGS) tests/example.cpp $(LDFLAGS) 
