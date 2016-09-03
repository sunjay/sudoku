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
    // Each value is in the range 1 to BOARD_SIZE+1
    // true if a value is available, false if a value cannot be used on this tile
    bool possibleValues[BOARD_SIZE]; /* Not used if value != 0 */
    // A cache of the number of true values in possibleValues
    int possibleCount; /* Not used if value != 0 */
} Tile;

typedef struct {
    Tile* tiles[BOARD_SIZE];
} SudokuBoard;

// Board initialization
void emptySudokuBoard(SudokuBoard*);

// Board retrieval methods
void getBoardTile(SudokuBoard*, int, int, Tile*);
void getBoardRow(SudokuBoard*, int, Tile[BOARD_SIZE]);
void getBoardColumn(SudokuBoard*, int, Tile[BOARD_SIZE]);
void getBoardBox(SudokuBoard*, int, Tile[BOX_SIZE][BOX_SIZE]);
void getTileBox(SudokuBoard*, int, int, Tile[BOX_SIZE][BOX_SIZE]);

// Board manipulation methods
void placeSudokuValue(SudokuBoard*, int, int, short);

// Bulk board manipulation methods
void setBoardRow(SudokuBoard*, int, Tile[BOARD_SIZE]);
void setBoardRowValues(SudokuBoard*, int, short[BOARD_SIZE]);

// Utility functions
void copySudokuBoard(SudokuBoard*, SudokuBoard*);
double getBoardDifficultyRating(SudokuBoard*);

// Board validation methods
bool isValidBoard(SudokuBoard*);
bool isCompleteBoard(SudokuBoard*);

#endif
