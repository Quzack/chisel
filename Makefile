CC    = g++
FLAGS = -Iinc -lws2_32 -lz
SRC   = src/network/*.cc src/thread/*.cc src/*.cc

all:
	${CC} ${SRC} -o bin/server -Iinc ${FLAGS}