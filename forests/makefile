CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =
LIBRARY = input.h def.h commands.h bst.h

.PHONY: all clean

all: forests

forests: forests.o input.o commands.o bst.o
	$(CC) $(LDFLAGS) -o $@ $^

forests.o: forests.c $(LIBRARY)
	$(CC) $(CFLAGS) -c $<

bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c $<

commands.o: commands.c commands.h def.h
	$(CC) $(CFLAGS) -c $<

input.o: input.c input.h def.h
	$(CC) $(CFLAGS) -c $<

clean:
	@echo "Deleting files:"
	rm -f *.o
	rm -f forests