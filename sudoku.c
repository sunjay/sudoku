/**
 * Sudoku Board Primitives
 *
 * Author: Sunjay Varma (www.sunjay.ca)
 */
#include <stdlib.h>
#include <stdbool.h>

#include "sudoku.h"

/**
 * Generates a completely empty, all zeros board
 *
 * Should always be freed after use.
 */
SudokuBoard* newSudokuBoard() {
    SudokuBoard* sudoku = malloc(sizeof(SudokuBoard));
    if (!sudoku) {
        return NULL;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        sudoku->tiles[i] = calloc(BOARD_SIZE, sizeof(Tile));
        if (!sudoku->tiles[i]) {
            return NULL;
        }
        for (int j = 0; j < BOARD_SIZE; j++) {
            // There are BOARD_SIZE possible values for every tile on
            // an empty board
            for (int p = 0; p < BOARD_SIZE; p++) {
                sudoku->tiles[i][j].possibleValues[p] = true;
            }
            sudoku->tiles[i][j].possibleCount = BOARD_SIZE;
        }
    }
    return sudoku;
}

/**
 * Copies a board and all of its values
 *
 * Should always be freed after use.
 */
SudokuBoard* copySudokuBoard(SudokuBoard* board) {
    SudokuBoard* copy = malloc(sizeof(SudokuBoard));
    if (!copy) {
        return NULL;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        copy->tiles[i] = malloc(BOARD_SIZE*sizeof(Tile));
        for (int j = 0; j < BOARD_SIZE; j++) {
            copy->tiles[i][j] = board->tiles[i][j];
        }
    }
    return copy;
}

/**
 * Frees a board from memory
 */
void freeSudokuBoard(SudokuBoard* board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        free(board->tiles[i]);
    }
    free(board);
}

/**
 * Generates a valid, completely filled, pre-solved, random board
 *
 * Currently only supports b
 *
 * Should always be freed after use.
 */
/*SudokuBoard* solvedSudokuBoard() {
    // A presolved board to shuffle around
    SudokuBoard* board = newSudokuBoard();
    if (board == NULL) {
        return NULL;
    }

    short row0[BOARD_SIZE] = {1, 2, 3,  4, 5, 6,  7, 8, 9};
    short row1[BOARD_SIZE] = {4, 5, 6,  7, 8, 9,  1, 2, 3};
    short row2[BOARD_SIZE] = {7, 8, 9,  1, 2, 3,  4, 5, 6};

    short row3[BOARD_SIZE] = {2, 3, 4,  5, 6, 7,  8, 9, 1};
    short row4[BOARD_SIZE] = {5, 6, 7,  8, 9, 1,  2, 3, 4};
    short row5[BOARD_SIZE] = {8, 9, 1,  2, 3, 4,  5, 6, 7};

    short row6[BOARD_SIZE] = {3, 4, 5,  6, 7, 8,  9, 1, 2};
    short row7[BOARD_SIZE] = {6, 7, 8,  9, 1, 2,  3, 4, 5};
    short row8[BOARD_SIZE] = {9, 1, 2,  3, 4, 5,  6, 7, 8};

    //TODO: Shuffle rows and columns randomly
    setBoardRow(board, 0, row0);
    setBoardRow(board, 1, row1);
    setBoardRow(board, 2, row2);
    setBoardRow(board, 3, row3);
    setBoardRow(board, 4, row4);
    setBoardRow(board, 5, row5);
    setBoardRow(board, 6, row6);
    setBoardRow(board, 7, row7);
    setBoardRow(board, 8, row8);
    return board;
}*/

/**
 * Places a value on the sudoku board. Updates all related possible value
 * caches and their counts.
 */
void placeSudokuValue(SudokuBoard* board, int row_i, int col_i, short value) {
    // Place the value on its tile
    board->tiles[row_i][col_i].value = value;

    if (value == 0) {
        return;
    }

    // The start index of this tile's box
    int boxRowStart = (row_i / BOX_SIZE) * BOX_SIZE;
    int boxColStart = (col_i / BOX_SIZE) * BOX_SIZE;

    // The index at which this value will be found in all possibleValue arrays
    int valueIndex = value - 1;

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Update items in the same row
        Tile* rowItem = &(board->tiles[row_i][i]);
        if (rowItem->possibleValues[valueIndex]) {
            // Tile was available, now it is not
            rowItem->possibleValues[valueIndex] = false;
            rowItem->possibleCount--;
        }

        // Update items in the same column
        Tile* colItem = &(board->tiles[i][col_i]);
        if (colItem->possibleValues[valueIndex]) {
            // Tile was available, now it is not
            colItem->possibleValues[valueIndex] = false;
            colItem->possibleCount--;
        }

        // Update items in the same box
        Tile* boxItem = &(board->tiles[boxRowStart + i / BOX_SIZE][boxColStart + i % BOX_SIZE]);
        if (boxItem->possibleValues[valueIndex]) {
            // Tile was available, now it is not
            boxItem->possibleValues[valueIndex] = false;
            boxItem->possibleCount--;
        }
    }
}

/**
 * Sets all items in a single board row to items
 *
 * items must be exactly BOARD_SIZE size
 */
void setBoardRow(SudokuBoard* board, int row_i, Tile* items) {
    Tile* row = board->tiles[row_i];
    for (int i = 0; i < BOARD_SIZE; i++) {
        row[i] = items[i];
    }
}

/**
 * Sets all values of all items in a single board row to items
 *
 * items must be exactly BOARD_SIZE size
 */
void setBoardRowValues(SudokuBoard* board, int row_i, short* items) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        placeSudokuValue(board, row_i, i, items[i]);
    }
}

/**
 * Returns an array of size BOARD_SIZE that represents a row of the board
 *
 * Should be freed after use
 */
Tile* getBoardRow(SudokuBoard* board, int row_i) {
    Tile* row = malloc(sizeof(Tile)*BOARD_SIZE);
    if (!row) {
        return NULL;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        row[i] = board->tiles[row_i][i];
    }
    return row;
}

/**
 * Returns an array of size BOARD_SIZE that represents a column of the board
 *
 * Should be freed after use
 */
Tile* getBoardColumn(SudokuBoard* board, int col_i) {
    Tile* col = malloc(sizeof(Tile)*BOARD_SIZE);
    if (!col) {
        return NULL;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        col[i] = board->tiles[i][col_i];
    }
    return col;
}

/**
 * Gets one of the boxes of the board of BOX_SIZE
 * Returns an array of array pointers that represents the box
 *
 * Box indexes work like this for BOARD_SIZE = 9:
 * 0 1 2
 * 3 4 5
 * 6 7 8
 *
 * Should be freed after use
 */
Tile** getBoardBox(SudokuBoard* board, int box_i) {
    Tile** box = malloc(sizeof(Tile*)*BOX_SIZE);
    if (!box) {
        return NULL;
    }

    int y_offset = (box_i / ((int)BOX_SIZE)) * BOX_SIZE;
    int x_offset = (box_i % ((int)BOX_SIZE)) * BOX_SIZE;
    for (int i = 0; i < BOX_SIZE; i++) {
        box[i] = malloc(sizeof(Tile)*BOX_SIZE);
        if (!box[i]) {
            return NULL; // yes this is a memory leak...
        }
        for (int j = 0; j < BOX_SIZE; j++) {
            box[i][j] = board->tiles[y_offset + i][x_offset + j];
        }
    }
    return box;
}

/**
 * Frees a box from memory
 */
void freeBox(Tile** box) {
    for (int i = 0; i < BOX_SIZE; i++) {
        free(box[i]);
    }
    free(box);
}

/**
 * Converts tile column/row position to box index
 */
int coordinatesToBoxIndex(int col_i, int row_i) {
    return (row_i / ((int)BOX_SIZE))*BOX_SIZE + col_i / ((int)BOX_SIZE);
}

/**
 * Gets the tiles in the box surrounding a given tile between
 *
 * col and row are the row and column index of the tile
 *
 * Should be freed after use
 */
Tile** getTileBox(SudokuBoard* board, int col_i, int row_i) {
    int box_i = coordinatesToBoxIndex(col_i, row_i);
    return getBoardBox(board, box_i);
}

/**
 * Checks for duplicates
 */
static bool isValid(Tile* row) {
    bool found[BOARD_SIZE] = {false, false, false, false, false, false, false, false, false};
    for (int i = 0; i < BOARD_SIZE; i++) {
        short value = row[i].value;
        if (value == 0) {
            continue;
        }
        if (found[value-1]) {
            return false;
        }
        found[value-1] = true;
    }
    return true;
}

/**
 * Validates a single tile by checking if there are repetitions in the
 * tile's row, column or box. This is much faster than checking the entire
 * board after every move.
 *
 * Ignores zero values.
 */
bool isValidTile(SudokuBoard* board, int col_i, int row_i) {
    Tile* row = getBoardRow(board, row_i);
    Tile* col = getBoardColumn(board, col_i);
    Tile** box = getTileBox(board, col_i, row_i);

    Tile box_values[BOARD_SIZE];
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            box_values[i*BOX_SIZE + j] = box[i][j];
        }
    }

    bool valid = isValid(row) && isValid(col) && isValid(box_values);

    free(col);
    free(row);
    freeBox(box);

    return valid;
}

/**
 * Validates whether the board is correct
 *
 * A board is incorrect if it has any repetitions
 */
bool isValidBoard(SudokuBoard* board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        // validate box
        Tile** box = getBoardBox(board, i);

        Tile box_values[BOARD_SIZE];
        for (int i = 0; i < BOX_SIZE; i++) {
            for (int j = 0; j < BOX_SIZE; j++) {
                box_values[i*BOX_SIZE + j] = box[i][j];
            }
        }
        freeBox(box);

        if (!isValid(box_values)) {
            return false;
        }

        // validate column
        Tile* col = getBoardColumn(board, i);

        bool colValid = isValid(col);
        free(col);

        if (!colValid) {
            return false;
        }

        // validate row
        Tile* row = getBoardRow(board, i);

        bool rowValid = isValid(row);
        free(row);

        if (!rowValid) {
            return false;
        }
    }
    return true;
}

/**
 * Returns whether you have a valid, complete board
 */
bool isCompleteBoard(SudokuBoard* board) {
    if (board == NULL) {
        return false;
    }
    // make sure everything is full
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Tile tile = board->tiles[i][j];
            if (tile.value == 0) {
                return false;
            }
        }
    }

    return isValidBoard(board);
}

/**
 * Rates the given board on a scale from 0.0 to 1.0
 * based on how difficult it is.
 */
double getBoardDifficultyRating(SudokuBoard* board) {
    // More empty tiles = more difficult
    double empty = 0;
    // Loop through all tiles and count how many are already
    // filled
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Tile tile = board->tiles[i][j];
            if (tile.value == 0) {
                empty += 1;
            }
        }
    }
    
    // Returns the inverse divided by the maximum board size
    return empty / (BOARD_SIZE * BOARD_SIZE);
}