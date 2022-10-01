CC    = g++
FLAGS = -Iinc -lws2_32
SRC   = src/network/*.cpp src/thread/*.cpp src/*.cpp

all:
	${CC} ${SRC} -o bin/server -Iinc ${FLAGS}