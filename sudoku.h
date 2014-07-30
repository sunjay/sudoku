#ifndef __SUDOKU_DEFS
#define __SUDOKU_DEFS

#include "boardproperties.h"

typedef struct {
	short* tiles[BOARD_SIZE];
} SudokuBoard;

// Board creation
SudokuBoard* newSudokuBoard();
SudokuBoard* solvedSudokuBoard();

// Board retrieval methods
short* getBoardRow(SudokuBoard* board, int row_i);
short* getBoardColumn(SudokuBoard* board, int col_i);
short** getBoardBox(SudokuBoard* board, int box_i);
short** getTileBox(SudokuBoard* board, int col_i, int row_i);
short* getTileSurroundings(SudokuBoard* board, int col_i, int row_i);

// Board set methods
void setBoardRow(SudokuBoard* board, int row_i, short* items);

// Utility functions
int coordinatesToBoxIndex(int col_i, int row_i);

#endif