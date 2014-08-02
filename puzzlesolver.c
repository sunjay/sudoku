#include <stdlib.h>

#include "sudoku.h"

struct EmptyTile {
	int row;
	int col;
	short* available_values;
	int available_count;
};

static void simpleSolver(SudokuBoard* board);
static SudokuBoard* guessSolver(SudokuBoard* board);
static struct EmptyTile* minimumTile(SudokuBoard* board);
static int count(short value, short items[], int length);

/**
 * Sudoku solving algorithm. Returns the solved board or NULL if for some
 * reason the board could not be solved.
 */
SudokuBoard* solveBoard(SudokuBoard* board) {
	// First algorithm
	simpleSolver(board);

	if (isCompleteBoard(board)) {
		return board;
	}

	if (isCompleteBoard(board)) {
		return board;
	}

	// Last resort brute-force guess
	return guessSolver(board);
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
			short* row = board->tiles[row_i];
			for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
				short value = row[col_i];
				// Is this an empty tile?
				if (value != 0) {
					continue;
				}

				short* available_values = getTileSurroundings(board, col_i, row_i);
				int available_count = count(0, available_values, BOARD_SIZE);
				if (available_count != 1) {
					free(available_values);
					continue; // more than one possible value to place here
				}

				// The only possible value
				short only_value = 0;
				// See the definition of getTileSurroundings for  clarification
				// on why this next part works
				for (int k = 0; k < BOARD_SIZE; k++) {
					if (available_values[k] == 0) {
						only_value = k + 1;
						break;
					}
				}

				foundValue = true;
				row[col_i] = only_value;

				free(available_values);
			}
		}
		if (!foundValue) {
			break;
		}
	}
}

/**
 * Attempts to guess on each empty tile based on the values available
 *
 * This is the brute force method. So it should (in time) return the
 * correct board. If the correct solution could still not be found,
 * returns NULL. That happens if all guesses were exhausted and still
 * no sure solution could be determined. Some solutions just don't work.
 */
static SudokuBoard* guessSolver(SudokuBoard* board) {
	// Get the tile with the minimum number of available values
	struct EmptyTile* minTile = minimumTile(board);
	if (minTile == NULL) {
		return NULL;
	}

	int row_i = minTile->row;
	int col_i = minTile->col;
	short* available_values = minTile->available_values;

	// Go through all available values
	for (int j = 0; j < BOARD_SIZE; j++) {
		// By convention, if this spot in the available_tiles is zero,
		// the corresponding number (based on the index) is no where in
		// the box, row or column surrounding this tile
		if (available_values[j] != 0) {
			continue; // skip values that already exist in the vicinity
		}

		// the value at this index is to be used as the guess
		short guess = j + 1;

		// retrieve a copy of a board
		SudokuBoard* copy = copySudokuBoard(board);

		// Make a guess
		copy->tiles[row_i][col_i] = guess;

		// Try to solve the board with this guess
		SudokuBoard* solved = solveBoard(copy);

		// If there's a solution, return it
		if (solved != NULL) {
			if (solved != copy) {
				free(copy);
			}
			return solved;
		}

		free(copy);
	}

	free(minTile);
	return NULL;
}

static int count(short value, short items[], int length) {
	int n = 0;
	for (int i = 0; i < length; i++) {
		if (items[i] == value) {
			n++;
		}
	}
	return n;
}

/**
 * Returns the tile with the minimum number of available values
 *
 * Returns NULL if no tiles were found
 */
static struct EmptyTile* minimumTile(SudokuBoard* board) {
	struct EmptyTile* minTile = malloc(sizeof(struct EmptyTile));
	if (!minTile) {
		return NULL;
	}
	bool foundMin = false;

	for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
		for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
			short value = board->tiles[row_i][col_i];
			if (value != 0) {
				continue; // tile is already filled, move on
			}

			short* available_values = getTileSurroundings(board, col_i, row_i);
			int available_count = count(0, available_values, BOARD_SIZE);

			if (!foundMin || available_count < minTile->available_count) {
				if (foundMin && minTile->available_values != NULL) {
					free(minTile->available_values);
				}

				minTile->row = row_i;
				minTile->col = col_i;
				minTile->available_values = available_values;
				minTile->available_count = available_count;

				foundMin = true;
			}
			else {
				free(available_values);
			}
		}
	}

	if (foundMin) {
		return minTile;
	}

	free(minTile);
	return NULL;
}
