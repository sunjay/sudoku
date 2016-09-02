#include <stdlib.h>

#include "sudoku.h"

struct TilePosition {
    int row;
    int col;
};

static void simpleSolver(SudokuBoard* board);
static SudokuBoard* eliminateSolver(SudokuBoard* board);
static struct TilePosition* minimumTile(SudokuBoard* board);

/**
 * Sudoku solving algorithm. Returns the solved board or NULL if for some
 * reason the board could not be solved.
 */
SudokuBoard* solveBoard(SudokuBoard* board) {
    // Very simple algorithm that continually fills in values with only one
    // possible value
    simpleSolver(board);

    if (isCompleteBoard(board)) {
        return board;
    }

    return eliminateSolver(board);
}

/**
 * The first tier and simplest solving algorithm.
 *
 * This algorithm goes through all empty tiles and checks what values
 * are available/valid for each tile. If there is only one possibility, that
 * is placed on that space.
 *
 * Solves as much as possible. Complete solution not guaranteed.
 *
 * Modifies the board in place.
 */
static void simpleSolver(SudokuBoard* board) {
    bool foundValue;
    while (true) {
        // These variables need to be inside the loop so that they
        // reset every time the algorithm goes back through the entire
        // board
        foundValue = false;
        for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
            Tile* row = board->tiles[row_i];
            for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
                Tile tile = row[col_i];
                // Is this an empty tile or at least a tile with only one possible move left?
                if (tile.value != 0 || tile.possibleCount != 1) {
                    continue;
                }

                bool* possibleValues = tile.possibleValues;

                // The only possible value is the one that is true
                short only_value = 0;
                for (int k = 0; k < BOARD_SIZE; k++) {
                    if (possibleValues[k]) {
                        only_value = k + 1;
                        break;
                    }
                }

                placeSudokuValue(board, row_i, col_i, only_value);
                foundValue = true;
            }
        }
        if (!foundValue) {
            break;
        }
    }
}

/**
 * Guesses what a position should be and then places a value based on that.
 * Continues by attempting to solve the board from there. If that works,
 * returns a fully filled out works. If anything fails along the way,
 * returns NULL.
 */
static SudokuBoard* eliminateSolver(SudokuBoard* board) {
    // Get the tile with the minimum number of possibilities
    struct TilePosition* minTile = minimumTile(board);
    if (minTile == NULL) {
        return NULL;
    }

    int row_i = minTile->row;
    int col_i = minTile->col;
    free(minTile);

    Tile tile = board->tiles[row_i][col_i];
    bool* possibleValues = tile.possibleValues;

    for (int j = 0; j < BOARD_SIZE; j++) {
        if (!possibleValues[j]) {
            continue; // skip values that already exist in the vicinity
        }

        // the value at this index is to be used as the guess
        short guess = j + 1;

        // retrieve a copy of a board
        SudokuBoard* copy = copySudokuBoard(board);

        // Make a guess
        placeSudokuValue(copy, row_i, col_i, guess);

        // Try to solve the board with this guess
        SudokuBoard* solved = solveBoard(copy);
        if (solved != copy) {
            freeSudokuBoard(copy);
        }

        // If there's a solution, return it
        if (solved != NULL) {
            return solved;
        }
    }
    return NULL;
}

/**
 * Returns the tile with the minimum number of possibilities
 *
 * Returns NULL if no tiles were found
 */
static struct TilePosition* minimumTile(SudokuBoard* board) {
    struct TilePosition* minTilePos = malloc(sizeof(struct TilePosition));
    if (!minTilePos) {
        return NULL;
    }
    Tile* minTile = NULL;

    for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
        for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
            Tile tile = board->tiles[row_i][col_i];
            if (tile.value != 0) {
                continue; // tile is already filled, move on
            }

            int possibleCount = tile.possibleCount;

            if (minTile == NULL || possibleCount < minTile->possibleCount) {
                minTile = &tile;
                minTilePos->row = row_i;
                minTilePos->col = col_i;
            }
        }
    }

    if (minTile != NULL) {
        return minTilePos;
    }

    free(minTilePos);
    return NULL;
}
