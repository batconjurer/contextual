
CFLAGS=-Wall -DNDEBUG -march=native -ffast-math -std=c++17 -g -O0 -fPIC -Iinclude
LDFLAGS=


all: example test

example: example.cpp
	g++ -o example $(CFLAGS) example.cpp $(LDFLAGS) 

test: tests/test.cpp
	g++ -o test $(CFLAGS) tests/test.cpp $(LDFLAGS)
