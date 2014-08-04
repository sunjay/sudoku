CC = gcc
CFLAGS = -g -O2 -std=c99 -Wall
OBJECTS = sudoku.o drawboard.o inputhandler.o

solvesudoku : $(OBJECTS) solvesudoku.o puzzlesolver.o boardparser.o
	$(CC) $(CFLAGS) solvesudoku.o puzzlesolver.o boardparser.o $(OBJECTS) -o solvesudoku

formatsudoku : $(OBJECTS) formatsudoku.o boardparser.o
	$(CC) $(CFLAGS) formatsudoku.o boardparser.o $(OBJECTS) -o formatsudoku

madness : madness.o
	$(CC) $(CFLAGS) madness.o -o madness

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	del *.exe *.o *~