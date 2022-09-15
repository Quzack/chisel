CC = g++
SRC = src/*.cpp

all:
	${CC} ${SRC} -o bin/server