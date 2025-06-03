###
### Makefile for the zap Project
###
### Author: Caleb Johnson
### Date: 11/8/23
MAKEFLAGS += -L

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3

# This rule links all .o files and creates an executable to run the program
zap: main.o HuffmanCoder.o HuffmanTreeNode.o BinaryIO.o ZapUtil.o
	${CXX} ${LDFLAGS} -o zap $^

# This rule will be used to build main.o
main.o: main.cpp HuffmanCoder.h

# This rule will be used to build HuffmanCoder.o
HuffmanCoder.o: HuffmanCoder.cpp HuffmanCoder.h HuffmanTreeNode.h BinaryIO.h ZapUtil.h

# This rule will be used to make the phaseOne.o file
phaseOne.o: phaseOne.cpp phaseOne.h HuffmanTreeNode.h

# The below rule will be used by unit_test
unit_test: unit_test_driver.o phaseOne.o HuffmanTreeNode.o
	$(CXX) $(CXXFLAGS) $^