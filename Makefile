CC    = g++
FLAGS = -Iinc -lws2_32
SRC   = src/*.cpp

all:
	${CC} ${SRC} -o bin/server ${FLAGS}