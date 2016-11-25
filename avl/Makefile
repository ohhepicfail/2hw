CXX = g++
CXXFLAGS= -c -Wall -Wextra -Wpedantic -std=c++14 -g3 -O2

all: avl clean
	./avl

avl: avl.o
	$(CXX) avl.o -o avl

avl.o: avl.cpp
	$(CXX) $(CXXFLAGS) avl.cpp



clean:
	rm -rf *.o prime

