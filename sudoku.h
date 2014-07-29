#ifndef __SUDOKU_DEFS
#define __SUDOKU_DEFS

#include "boardproperties.h"

typedef struct {
	short tiles[BOARD_SIZE*BOARD_SIZE];
} SudokuBoard;

// Board creation
SudokuBoard* newSudokuBoard();
SudokuBoard* solvedSudokuBoard();

// Board methods
short* getBoardRow(SudokuBoard* board, int row_i);
short* getBoardColumn(SudokuBoard* board, int col_i);
short* getBoardBox(SudokuBoard* board, int box_i);
short* getTileBox(SudokuBoard* board, int tile_i);
short* getTileSurroundings(SudokuBoard* board, int tile_i);

#endif