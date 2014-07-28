#ifndef __SUDOKU_DEFS
#define __SUDOKU_DEFS

#include "boardproperties.h"

typedef struct {
	short tiles[BOARD_SIZE*BOARD_SIZE];
} SudokuBoard;

// Board creation
SudokuBoard* newSudokuBoard();
SudokuBoard* solvedSudokuPuzzle();

// Board methods
short* getBoardRow(SudokuBoard* board, int row_i);
short* getBoardColumn(SudokuBoard* board, int col_i);

#endif