CFLAGS = -Wall -Wextra -std=c11 -O2
OBJECTS = quantization.o commands.o list.o queue.o tree.o input.o
LIBRARY = commands.h list.h queue.h tree.h input.h

.PHONY: all clean

all: quantization

quantization: $(OBJECTS)
		@ gcc -o quantization $(OBJECTS)

input.o: input.c input.h
		@ gcc $(CFLAGS) -c $<

tree.o: tree.c tree.h
		@ gcc $(CFLAGS) -c $<

queue.o: queue.c queue.h
		@ gcc $(CFLAGS) -c $<

list.o: list.c list.h
		@ gcc $(CFLAGS) -c $<

commands.o: commands.c commands.h
		@ gcc $(CFLAGS) -c $<

quantization.o: quantization.c $(LIBRARY)
		@ gcc $(CFLAGS) -c $<

clean:
		@ rm -f *.o quantization