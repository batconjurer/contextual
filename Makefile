
CFLAGS=-Wall -DNDEBUG -march=native -ffast-math -std=c++17 -O3 -fPIC -Iinclude
LDFLAGS=


all: example tests

example: tests/example.cpp
	g++ -o example $(CFLAGS) example.cpp $(LDFLAGS) 

tests: tests/test.cpp
	g++ -o test $(CFLAGS) tests/test.cpp $(LDFLAGS)