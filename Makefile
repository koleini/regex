CXX=g++
CXXFLAGS=-Wall -std=c++11
NAME=regex
LDLIBS=-lpcrecpp
FILES=$(NAME).cpp tcs.cpp

all: $(FILES)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(FILES) $(LDLIBS)

clean:
	rm -f $(NAME).o
