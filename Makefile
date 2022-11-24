CC=g++
CFLAGS=-std=c++14 -Wall -Wpedantic -O3 -march=native

all: speed_test

speed_test: speed.cpp jmatrix.hpp
	$(CC) $(CFLAGS) -o speed_test speed.cpp

assembly_listing: speed.cpp jmatrix.hpp
	$(CC) $(CFLAGS) -S -o convolution2d.s speed.cpp
