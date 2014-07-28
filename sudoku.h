#ifndef __SUDOKU_DEFS
#define __SUDOKU_DEFS

// The width/height of a single board and also the big board
#define BOX_SIZE 3
#define BOARD_SIZE BOX_SIZE*BOX_SIZE

typedef struct {
	short tiles[BOARD_SIZE*BOARD_SIZE];
} SudokuBoard;

#endif