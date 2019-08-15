CXX=g++
CXX_FLAGS=-Wall -Wextra -std=c++11

all: bwtstats

bwtstats: bwtstats.cpp
	$(CXX) -o $@ $< $(CXX_FLAGS)

