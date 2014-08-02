#ifndef __SUDOKU_DEFS
#define __SUDOKU_DEFS

#include <stdbool.h>

// The width/height of a single board and also the big board
#define BOX_SIZE 3
#define BOARD_SIZE (BOX_SIZE*BOX_SIZE)

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
void setBoardRow(SudokuBoard* board, int row_i, short items[]);

// Utility functions
int coordinatesToBoxIndex(int col_i, int row_i);
SudokuBoard* copySudokuBoard(SudokuBoard* board);

// Board validation methods
bool isValidTile(SudokuBoard* board, int col_i, int row_i);
bool isValidBoard(SudokuBoard* board);
bool isCompleteBoard(SudokuBoard* board);

#endif