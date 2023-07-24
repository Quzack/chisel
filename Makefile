CC    = g++
CVER  = -std=c++17
FLAGS = -Iinc -lws2_32 -lz
SRC   = src/network/*.cc src/thread/*.cc src/*.cc

all:
	${CC} ${SRC} -o bin/server -Iinc ${CVER} ${FLAGS}