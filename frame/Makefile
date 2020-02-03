BIN = kiddo
LIB = libkiddo.so
SOURCE = $(wildcard *.cpp)
OBJECT = $(patsubst %.cpp,%.o,$(SOURCE))
LIB_DIR = $(shell dirname `pwd`)/lib
SERVER = $(shell dirname `pwd`)/server
CC = g++
CFLAGS = -std=c++11 -pthread -Werror=return-type

$(BIN): main.cpp $(LIB)
	$(CC) $(CFLAGS) -I $(SERVER) -L $(LIB_DIR) -lkiddo $< -Wl,-rpath $(LIB_DIR) -o $@

$(LIB):
	$(MAKE) -C $(SERVER)

.PHONY: clean
clean:
	rm -rf *.out *.o *.so *.lib $(BIN)
