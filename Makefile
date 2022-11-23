CC=g++
CFLAGS=-std=c++11 -Wall -Wpedantic -O3 -march=native

all: speed_test

speed_test: speed.cpp
	$(CC) $(CFLAGS) -o speed_test speed.cpp
