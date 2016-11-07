CXX = g++
CXXFLAGS= -c -Wall -Wextra -Wpedantic -std=c++14 -g3

all: ukk clean
	./ukk

ukk: ukk.o ukk_test.o string_processing.o
	$(CXX) ukk.o ukk_test.o string_processing.o -o ukk

ukk_test.o: ukk_test.cpp
	$(CXX) $(CXXFLAGS) ukk_test.cpp

ukk.o: ukk.cpp
	$(CXX) $(CXXFLAGS) ukk.cpp

string_processing.o: string_processing.cpp
	$(CXX) $(CXXFLAGS) string_processing.cpp

clean:
	rm -rf *.o prime

