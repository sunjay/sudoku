/**
 * Sudoku Board Primitives
 *
 * Author: Sunjay Varma (www.sunjay.ca)
 */
#include <stdbool.h>

#include "sudoku.h"

/**
 * Converts tile column/row indexes to the appropriate index in the
 * SudokuBoard tiles property
 */
static int coordinatesToTileIndex(int row_i, int col_i) {
    return row_i * BOARD_SIZE + col_i;
}

/**
 * Initializes a sudoku board to be a completely empty (all zeros) board
 * Does not allocate any memory
 */
void emptySudokuBoard(SudokuBoard* board) {
    int index;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            index = coordinatesToTileIndex(i, j);
            // There are BOARD_SIZE possible values for every tile on
            // an empty board
            for (int p = 0; p < BOARD_SIZE; p++) {
                board->tiles[index].possibleValues[p] = true;
            }
            board->tiles[index].possibleCount = BOARD_SIZE;
        }
    }
}

/**
 * Copies a board and all of its values to the provided copy
 * Does not allocate any memory
 */
void copySudokuBoard(SudokuBoard* board, SudokuBoard* copy) {
    // Let the compiler generate the copy code
    *copy = *board;
}

/**
 * Places a value on the sudoku board. Updates all related possible value
 * caches and their counts.
 */
void placeSudokuValue(SudokuBoard* board, int row_i, int col_i, short value) {
    // Place the value on its tile
    int index = coordinatesToTileIndex(row_i, col_i);
    board->tiles[index].value = value;

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
        index = coordinatesToTileIndex(row_i, i);
        Tile* rowItem = &(board->tiles[index]);
        if (rowItem->possibleValues[valueIndex]) {
            // Tile was available, now it is not
            rowItem->possibleValues[valueIndex] = false;
            rowItem->possibleCount--;
        }

        // Update items in the same column
        index = coordinatesToTileIndex(i, col_i);
        Tile* colItem = &(board->tiles[index]);
        if (colItem->possibleValues[valueIndex]) {
            // Tile was available, now it is not
            colItem->possibleValues[valueIndex] = false;
            colItem->possibleCount--;
        }

        // Update items in the same box
        index = coordinatesToTileIndex(boxRowStart + i / BOX_SIZE,
                    boxColStart + i % BOX_SIZE);
        Tile* boxItem = &(board->tiles[index]);
        if (boxItem->possibleValues[valueIndex]) {
            // Tile was available, now it is not
            boxItem->possibleValues[valueIndex] = false;
            boxItem->possibleCount--;
        }
    }
}

/**
 * Sets all items in a single board row to items
 */
void setBoardRow(SudokuBoard* board, int row_i, Tile items[BOARD_SIZE]) {
    int index;

    for (int i = 0; i < BOARD_SIZE; i++) {
        index = coordinatesToTileIndex(row_i, i);
        board->tiles[index] = items[i];
    }
}

/**
 * Sets all values of all items in a single board row to items
 */
void setBoardRowValues(SudokuBoard* board, int row_i, short items[BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        placeSudokuValue(board, row_i, i, items[i]);
    }
}

/**
 * Gets a single Tile at the given row_i and col_i position
 * row_i and col_i are indexes where 0 <= i < BOARD_SIZE
 * Tile is a pointer to a single Tile
 */
void getBoardTile(SudokuBoard* board, int row_i, int col_i, Tile* tile) {
    int index = coordinatesToTileIndex(row_i, col_i);
    // The compiler will generate appropriate copy code
    *tile = board->tiles[index];
}

/**
 * Copies the values from the row at the given index into the given row
 * Performs no allocation. row must be large enough for BOARD_SIZE Tiles
 */
void getBoardRow(SudokuBoard* board, int row_i, Tile row[BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        getBoardTile(board, row_i, i, &row[i]);
    }
}

/**
 * Copies the values from the column at the given index into the given col
 * Performs no allocation. col must be large enough for BOARD_SIZE Tiles
 */
void getBoardColumn(SudokuBoard* board, int col_i, Tile col[BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        getBoardTile(board, i, col_i, &col[i]);
    }
}

/**
 * Copies the values of the given "box index" into the provided box
 * box must be an array of array pointers that represents the box
 * Does not allocate any memory
 *
 * Box indexes work like this for BOARD_SIZE = 9:
 * 0 1 2
 * 3 4 5
 * 6 7 8
 */
void getBoardBox(SudokuBoard* board, int box_i, Tile box[BOX_SIZE][BOX_SIZE]) {
    int y_offset = (box_i / ((int)BOX_SIZE)) * BOX_SIZE;
    int x_offset = (box_i % ((int)BOX_SIZE)) * BOX_SIZE;
    for (int i = 0; i < BOX_SIZE; i++) {
        for (int j = 0; j < BOX_SIZE; j++) {
            getBoardTile(board, y_offset + i, x_offset + j, &box[i][j]);
        }
    }
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
 * Validates whether the board is correct
 *
 * A board is incorrect if it has any repetitions
 */
bool isValidBoard(SudokuBoard* board) {
    Tile row[BOARD_SIZE];
    Tile col[BOARD_SIZE];
    Tile box[BOX_SIZE][BOX_SIZE];

    for (int i = 0; i < BOARD_SIZE; i++) {
        // validate box
        getBoardBox(board, i, box);

        Tile box_values[BOARD_SIZE];
        for (int i = 0; i < BOX_SIZE; i++) {
            for (int j = 0; j < BOX_SIZE; j++) {
                box_values[i*BOX_SIZE + j] = box[i][j];
            }
        }

        if (!isValid(box_values)) {
            return false;
        }

        // validate column
        getBoardColumn(board, i, col);
        bool colValid = isValid(col);

        if (!colValid) {
            return false;
        }

        // validate row
        getBoardRow(board, i, row);
        bool rowValid = isValid(row);

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
    int index;

    // make sure everything is full
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            index = coordinatesToTileIndex(i, j);

            if (board->tiles[index].value == 0) {
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
    int index;

    // More empty tiles = more difficult
    double empty = 0;
    // Loop through all tiles and count how many are already filled
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            index = coordinatesToTileIndex(i, j);

            if (board->tiles[index].value == 0) {
                empty += 1;
            }
        }
    }
    
    // Returns the inverse divided by the maximum board size
    return empty / (BOARD_SIZE * BOARD_SIZE);
}

