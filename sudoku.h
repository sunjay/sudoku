#ifndef __SUDOKU_DEFS
#define __SUDOKU_DEFS

#include <stdbool.h>

// The width/height of a single board and also the big board
#define BOX_SIZE 3
#define BOARD_SIZE (BOX_SIZE*BOX_SIZE)

typedef struct {
	// The value of the tile
	short value;
	// Each position in this array represents a valid value for this tile
	// true if that position is available, false otherwise
	bool possibleValues[BOARD_SIZE]; /* Not used if value != 0 */
	// A cache of the number of true values in possibleValues
	int possibleCount; /* Not used if value != 0 */
} Tile;

typedef struct {
	Tile** tiles;
} SudokuBoard;

// Board creation
SudokuBoard* newSudokuBoard();
SudokuBoard* solvedSudokuBoard();

// Board deletion
void freeSudokuBoard(SudokuBoard* board);
void freeBox(Tile** box);

// Board retrieval methods
Tile* getBoardRow(SudokuBoard* board, int row_i);
Tile* getBoardColumn(SudokuBoard* board, int col_i);
Tile** getBoardBox(SudokuBoard* board, int box_i);
Tile** getTileBox(SudokuBoard* board, int col_i, int row_i);
Tile* getTileSurroundings(SudokuBoard* board, int col_i, int row_i);

// Board set methods
void setBoardRow(SudokuBoard* board, int row_i, Tile* items);
void setBoardRowValues(SudokuBoard* board, int row_i, short* items);

// Utility functions
int coordinatesToBoxIndex(int col_i, int row_i);
SudokuBoard* copySudokuBoard(SudokuBoard* board);

// Board validation methods
bool isValidTile(SudokuBoard* board, int col_i, int row_i);
bool isValidBoard(SudokuBoard* board);
bool isCompleteBoard(SudokuBoard* board);

#endif