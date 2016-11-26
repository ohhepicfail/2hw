CXX = g++
CXXFLAGS= -c -Wall -Wextra -Wpedantic -std=c++14 -g3 -O2

all: avl clean
	./avl

avl: avl.o avl_test.o
	$(CXX) avl.o avl_test.o -o avl

avl.o: avl.cpp
	$(CXX) $(CXXFLAGS) avl.cpp

avl_test.o: avl_test.cpp
	$(CXX) $(CXXFLAGS) avl_test.cpp



clean:
	rm -rf *.o prime

