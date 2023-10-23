CC    = g++
CVER  = -std=c++17
FLAGS = -Iinc -lws2_32 -lz
SRC   = src/*.cc 

all:
	${CC} ${SRC} -o bin/server ${CVER} ${FLAGS}