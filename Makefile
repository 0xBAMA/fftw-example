all: build run

build:
	g++ fftw_example.c -lfftw3

run:
	./a.out
