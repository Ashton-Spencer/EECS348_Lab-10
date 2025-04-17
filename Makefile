CXX = g++
CXXFLAGS = -std=c++17 -Wall

all:
	$(CXX) $(CXXFLAGS) main.cpp calculator.cpp -o calculator

clean:
	rm -f calculator