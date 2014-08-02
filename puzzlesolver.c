#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

static int count(short value, short items[], int length);
static void simpleSolver(SudokuBoard* board);
static void smartSolver(SudokuBoard* board);
static SudokuBoard* guessSolver(SudokuBoard* board);

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

	// Second algorithm
	smartSolver(board);

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
 * Finds the solution to the given tile by narrowing it down from all possible
 * values for this position. Does so by checking the surrounding rows and columns
 * (still in the same box).
 *
 * Only returns a single perfect value. A value is perfect if no other
 * value could be put into this tile (based on the surroundings).
 *
 * Returns the found value if any, or zero if no perfect value is found.
 */
static short narrowSolveTile(SudokuBoard* board, int col_i, int row_i) {
	short result = 0;

	// Go through all possible values that can be validly placed on this tile
	short* available_values = getTileSurroundings(board, col_i, row_i);
	for (int i = 0; i < BOARD_SIZE; i++) {
		// By convention, if this spot in the available_tiles is zero,
		// the corresponding number (based on the index) is no where in
		// the box, row or column surrounding this tile
		if (available_values[i] != 0) {
			continue; // skip values that already exist in the vicinity
		}

		// The value that corresponds to this index
		short value = i + 1;
		value += 1;
	}

	free(available_values);
	return result;
}

/**
 * Attempts to solve the board based on the surrounding rows and columns
 * (in the same box). Only makes complete 100% confidence
 * choices. (No guesses)
 *
 * This algorithm uses the surrounding tiles instead of just the ones
 * in the same row.
 */
static void smartSolver(SudokuBoard* board) {
	for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
		for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
			short tile_value = board->tiles[row_i][col_i];
			if (tile_value != 0) {
				continue; // tile is already filled, move on
			}

			// Must be dealing with an empty tile
			// Narrow it down to one solution
			short value = narrowSolveTile(board, col_i, row_i);

			// if a solution was found
			if (value != 0) {
				// Place the value and move on
				board->tiles[row_i][col_i] = value;
			}
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
	for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
		for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
			short value = board->tiles[row_i][col_i];
			if (value != 0) {
				continue; // tile is already filled, move on
			}

			short* available_values = getTileSurroundings(board, col_i, row_i);
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

				system("cls");
				printf("Guessing %d at (%d, %d)\n", guess, col_i, row_i);
				for (int q = 0; q < BOARD_SIZE; q++) {
					for (int t = 0; t < BOARD_SIZE; t++) {
						printf("%d", copy->tiles[q][t]);
					}
					printf("\n");
				}

				// Try to solve the board
				SudokuBoard* solved = solveBoard(copy);

				// If there's a solution, return it
				if (solved != NULL) {
					if (solved != copy) {
						free(copy);
					}
					free(solved);
					return solved;
				}

				free(copy);
			}
		}
	}
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
