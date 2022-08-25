CC = gcc
CFLAGS =-g -Wall -Wextra -pedantic `pkg-config --cflags sdl2` -ggdb #-O3
LDFLAGS =`pkg-config --libs sdl2`
OBJS = main.o 

all: clean main

main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) #-lncurses

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf *.o main
