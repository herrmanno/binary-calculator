SRC = src/binary.cpp src/parser.cpp
CCFLAGS = -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-documentation-unknown-command -std=c++14

all: binary test

binary: src/*.cpp src/*.hpp
	clang++ $(CCFLAGS) -o binary src/main.cpp $(SRC)

test: test/*cpp src/*.cpp src/*.hpp
	clang++ $(CCFLAGS) -o testBinary test/test.cpp $(SRC)

runtest: test
	./testBinary
