BIN = kiddo
LIB = libkiddo.so
SOURCE = $(wildcard *.cpp)
OBJECT = $(patsubst %.cpp,%.o,$(SOURCE))
LIB_DIR = $(shell dirname `pwd`)/lib
HEADER = $(shell dirname `pwd`)/server
CC = g++
CFLAGS = -std=c++11 -pthread -Werror=return-type

$(BIN):
	$(CC) $(CFLAGS) -I $(HEADER) -L $(LIB_DIR) -lkiddo main.cpp -Wl,-rpath $(LIB_DIR) -o $@

.PHONY: clean
clean:
	rm -rf *.out *.o *.so *.lib $(BIN)
