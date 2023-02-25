PROGRAMS = prog 
CC = gcc
CFLAGS = -Wall -g
all: $(PROGRAMS)
clean:
	rm -f *.o
# C compilations
prog.o: prog.c prog.h
	$(CC) $(CFLAGS) -c prog.c 

# Executable programs
prog: prog.o
	$(CC) $(CFLAGS) -o prog prog.o