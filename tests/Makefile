all: tests

tests: tests.cpp $(wildcard ../unilib/*.cpp) $(wildcard ../unilib/*.h)
	g++ $< $(wildcard ../unilib/*.cpp) -std=c++11 -I ../unilib -W -Wall -O2 -g -o $@

.PHONY: clean
clean:
	rm -f tests
