#ifndef __SUDOKU_DEFS
#define __SUDOKU_DEFS

#include "boardproperties.h"

typedef struct {
	short tiles[BOARD_SIZE*BOARD_SIZE];
} SudokuBoard;

SudokuBoard* newSudokuBoard();
SudokuBoard* sudokuPuzzle();

#endif