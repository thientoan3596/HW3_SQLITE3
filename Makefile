CC = gcc
CCFLAGS = -lsqlite3 -Wall -Wextra -std=c99 -pedantic -g -I.
DEPS = libmysqlite3.h
OBJ = main.o libmysqlite3.o


.PHONY: all
all: myapp

myapp: $(OBJ)
	$(CC) $^ $(CCFLAGS) -o $@

%.o: %.c $(DEPS)
	$(CC) -c $(CCFLAGS) $<

.PHONY: clean
clean:
	rm -rf *.o myapp
