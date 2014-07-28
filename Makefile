CC = gcc
CFLAGS = -g -O2 -std=c99 -Wall
OBJECTS = sudoku.o drawboard.o

playsudoku : $(OBJECTS) playsudoku.o
	$(CC) $(CFLAGS) playsudoku.o $(OBJECTS) -o playsudoku

solvesudoku : $(OBJECTS) solvesudoku.o puzzlesolver.o
	$(CC) $(CFLAGS) solvesudoku.o puzzlesolver.o $(OBJECTS) -o solvesudoku

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	del *.exe *.o *~