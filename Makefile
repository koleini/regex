CXX=g++
CXXFLAGS=-Wall -std=c++11
NAME=regex
LDLIBS=-lpcrecpp

all: $(NAME).cpp 
	$(CXX) $(CXXFLAGS) -o $(NAME) $(NAME).cpp $(LDLIBS)

clean:
	rm -f $(NAME).o
