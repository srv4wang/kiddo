LIB = libkiddo.so
SOURCE = $(wildcard *.cpp)
OBJECT = $(patsubst %.cpp,%.o,$(SOURCE))
DIR = $(shell dirname `pwd`)/lib
CC = g++
CFLAGS = -std=c++11 -pthread

echo.out: $(LIB)
	$(CC) $(CFLAGS) -L $(DIR) -lkiddo echo.cc -Wl,-rpath $(DIR) -o $@

$(LIB): $(OBJECT)
	$(CC) $(CFLAGS) -fPIC -shared -o $(DIR)/$@ $^ 
%.o: %.cpp
	$(CC) $(CFLAGS) -fPIC -c -o $@ $^

.PHONY: clean install
clean:
	rm -rf *.out *.o *.so *.lib
install:
	install -m 755 $(LIB) /usr/local/lib/
