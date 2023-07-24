CC=g++ -std=c++11
CFLAGS=-g -Wall -O3 -fast
IFLAGS=-I/opt/X11/include
LFLAGS=-L/opt/X11/lib -lX11
all: shift3

shift3: shift3.cc graphics.cc polynomial.cc lamination.cc green.cc tautological.cc julia.cc mandelbrot.cc interface.cc
	$(CC) $(CFLAGS) $(IFLAGS) -o shift3 shift3.cc $(LFLAGS) -lm

clean: rm shift3
