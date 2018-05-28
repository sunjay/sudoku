CFLAGS = -g -O3 -std=c99 -Wall
OBJECTS = sudoku.o drawboard.o inputhandler.o boardparser.o

all: solvesudoku formatsudoku

solvesudoku : $(OBJECTS) solvesudoku.o puzzlesolver.o
	$(CC) $(CFLAGS) solvesudoku.o puzzlesolver.o $(OBJECTS) -o solvesudoku

timesolvesudoku : $(OBJECTS) timesolvesudoku.o puzzlesolver.o
	$(CC) $(CFLAGS) timesolvesudoku.o puzzlesolver.o $(OBJECTS) -lrt -o timesolvesudoku

formatsudoku : $(OBJECTS) formatsudoku.o
	$(CC) $(CFLAGS) formatsudoku.o $(OBJECTS) -o formatsudoku

madness : madness.o
	$(CC) $(CFLAGS) madness.o -o madness

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.exe *.o *~
