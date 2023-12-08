# Author: Javier Izquierdo Hernandez (javizqh@gmail.com)
# Also: j.izquierdoh.2021@alumnos.urjc.es
# Date: 2023-10-18

# If compile with debug change to this $(CC) -c -DDEBUG -o $@ $< $(CFLAGS)

CC=gcc
CFLAGS=-I.
DEPS =

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: day8

day8: src/main.c 
	gcc -g -Wall -o build/day8 src/main.c

clean:
	rm -f src/*.o build/day8
