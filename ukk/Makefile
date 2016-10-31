CXX = g++
CXXFLAGS= -c -Wall -Wextra -Wpedantic -std=c++14 -g3 -O2 -pg

all: ukk clean
	./ukk

ukk: ukk.o ukk_test.o
	$(CXX) ukk.o ukk_test.o -o ukk -pg

ukk_test.o: ukk_test.cpp
	$(CXX) $(CXXFLAGS) ukk_test.cpp

ukk.o: ukk.cpp
	$(CXX) $(CXXFLAGS) ukk.cpp

clean:
	rm -rf *.o prime

