#include <stdlib.h> // NULL

#include "sudoku.h"

struct TilePosition {
    int row;
    int col;
};

static void simpleSolver(SudokuBoard*);
static int eliminateSolver(SudokuBoard*);
static int minimumTile(SudokuBoard*, struct TilePosition*);

/**
 * Sudoku solving algorithm.
 *
 * Returns 0 if solving was successful, -1 otherwise
 */
int solveBoard(SudokuBoard* board) {
    // Very simple algorithm that continually fills in values with only one
    // possible value
    simpleSolver(board);

    if (isCompleteBoard(board)) {
        return 0;
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
    // foundValue is used to track whether a tile was solved this time
    // through the board
    // We continue going through all the available tiles on the board
    // until we can't solve any more values with this method
    bool foundValue;

    Tile tile;
    while (true) {
        // These variables need to be inside the loop so that they
        // reset every time the algorithm goes back through the entire
        // board
        foundValue = false;

        for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
            for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
                getBoardTile(board, row_i, col_i, &tile);

                // Is this an empty tile or at least a tile with
                // only one possible move left?
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

        // Unable to solve anything else, stop trying
        if (!foundValue) {
            break;
        }
    }
}

/**
 * Second tier of solving (intelligent brute force)
 * Makes an intelligent guess about what position to guess on
 * Guesses an appropriate value and attempts to solve from the first tier
 * using the modified board
 * If no solution is found, the initial guess is undone and the search
 * continues
 * Only fails if all possible solutions are exhausted.
 *
 * Returns 0 if a solution was found, -1 otherwise
 */
static int eliminateSolver(SudokuBoard* board) {
    // Get the tile with the minimum number of possibilities
    // This is the most efficient place to start guessing because
    // if we guess wrong we will have the fewest number of alternatives
    // to search
    // On the otherhand, if we chose the tile with the maximum number
    // of possible values, we elimate more possible routes by guessing
    // right. I'm just betting that we'll guess wrong more often then
    // we guess right since there are more wrong numbers than right ones.
    struct TilePosition minTile;
    if (minimumTile(board, &minTile) == -1) {
        return -1;
    }

    int row_i = minTile.row;
    int col_i = minTile.col;

    Tile tile;
    getBoardTile(board, row_i, col_i, &tile);

    bool* possibleValues = tile.possibleValues;

    SudokuBoard copy;
    for (int j = 0; j < BOARD_SIZE; j++) {
        if (!possibleValues[j]) {
            // Skip values that cannot possibly be solutions
            continue;
        }

        // The value at this index is to be used as the guess
        // Each index in possibleValues represents a value from 1-9
        // Adding 1 to the index produces the value
        short guess = j + 1;

        // retrieve a copy of a board
        copySudokuBoard(board, &copy);

        // Make a guess
        placeSudokuValue(&copy, row_i, col_i, guess);

        // Try to solve the board with this guess
        if (solveBoard(&copy) == 0) {
            // copy the solution back onto the other board
            copySudokuBoard(&copy, board);
            return 0;
        }
    }

    // Exhausted this route. It's possible that a guess from before
    // was incorrect
    return -1;
}

/**
 * Returns the tile with the minimum number of possibilities
 *
 * Returns 0 if successful, -1 if no tile was found
 */
static int minimumTile(SudokuBoard* board, struct TilePosition* minTilePos) {
    Tile* minTile = NULL;

    Tile tile;
    for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
        for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
            getBoardTile(board, row_i, col_i, &tile);
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

    if (minTile == NULL) {
        return -1;
    }

    return 0;
}

