CC    = g++
FLAGS = -Iinc -lws2_32 -lz
SRC   = src/*.cc src/network/*.cc src/thread/*.cc

all:
	${CC} ${SRC} -o bin/server -Iinc ${FLAGS}