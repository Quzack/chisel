CC    = g++
FLAGS = -Iinc
SRC   = src/*.cpp

all:
	${CC} ${SRC} -o bin/server ${FLAGS}