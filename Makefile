CC    = g++
FLAGS = -Iinc -lws2_32
SRC   = src/network/*.cc src/thread/*.cc src/*.cc

all:
	${CC} ${SRC} -o bin/server -Iinc ${FLAGS}