CXX = g++
CXXFLAGS = -c -Wall -Wextra -Wpedantic -g3 -std=c++14 

all: list tree clean	

tree: tree.o list.o tree_testing.o
	$(CXX) tree.o list.o tree_testing.o -o tree

list: list_testing.o list.o
	$(CXX) list_testing.o list.o -o list

list_testing.o: list_testing.cpp
	$(CXX) $(CXXFLAGS) list_testing.cpp

list.o: list.cpp
	$(CXX) $(CXXFLAGS) list.cpp

tree.o: tree.cpp
	$(CXX) $(CXXFLAGS) tree.cpp

tree_testing.o: tree_testing.cpp
	$(CXX) $(CXXFLAGS) tree_testing.cpp

clean:
	rm -rf *.o
