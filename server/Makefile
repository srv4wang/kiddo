LIB = libkiddo.so
SOURCE = $(wildcard *.cpp)
OBJECT = $(patsubst %.cpp,%.o,$(SOURCE))
DIR = $(shell pwd)
CC = g++
CFLAGS = -std=c++11 -pthread

ECHO: $(LIB)
	$(CC) $(CFLAGS) -L $(DIR) -lkiddo echo.cc -Wl,-rpath $(DIR)

$(LIB): $(OBJECT)
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^ 
%.o: %.cpp
	$(CC) $(CFLAGS) -fPIC -c -o $@ $^

.PHONY: clean install
clean:
	rm -rf *.out *.o *.so *.lib
install:
	install -m 755 $(LIB) /usr/local/lib/
