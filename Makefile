CC = gcc
CFLAGS = -g -O2 -std=c99 -Wall
OBJECTS = sudoku.o drawboard.o

sudoku : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o sudoku

%.o : %.c
	$(CC) $(CFLAGS) -c $<