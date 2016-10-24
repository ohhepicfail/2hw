CXX = g++
CXXFLAGS = -c -Wall -Wextra -Wpedantic -g3 -std=c++14 -pg -O2

all: list tree treap clean	

tree: tree.o list.o tree_testing.o
	$(CXX) tree.o list.o tree_testing.o -o tree

list: list_testing.o list.o
	$(CXX) list_testing.o list.o -o list

treap: queue.o treap.o treap_testing.o
	$(CXX) queue.o treap.o treap_testing.o -o treap -pg

list_testing.o: list_testing.cpp
	$(CXX) $(CXXFLAGS) list_testing.cpp

list.o: list.cpp
	$(CXX) $(CXXFLAGS) list.cpp

tree.o: tree.cpp
	$(CXX) $(CXXFLAGS) tree.cpp

tree_testing.o: tree_testing.cpp
	$(CXX) $(CXXFLAGS) tree_testing.cpp

treap.o: treap.cpp
	$(CXX) $(CXXFLAGS) treap.cpp

treap_testing.o: treap_testing.cpp
	$(CXX) $(CXXFLAGS) treap_testing.cpp

queue.o :queue.cpp
	$(CXX) $(CXXFLAGS) queue.cpp

clean:
	rm -rf *.o
