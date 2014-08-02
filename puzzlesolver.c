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
 * Attempts to solve the board based on patterns. Only makes complete
 * 100% confidence choices. (No guesses)
 *
 * This algoirthm uses the surrounding tiles instead of just the ones
 * in the same row.
 */
static void smartSolver(SudokuBoard* board) {
	for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
		for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
			short value = board->tiles[row_i][col_i];
			if (value != 0) {
				continue; // tile is already filled, move on
			}

			// Must be dealing with an empty tile
			short* available_values = getTileSurroundings(board, col_i, row_i);
			free(available_values);
		}
	}
}

/**
 * Attempts to guess on each empty tile based on the values available
 *
 * This is the brute force method. So it should (in time) return the
 * correct board. If the correct solution could still not be found,
 * returns NULL.
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
				// by convention, zero means that this value is available and valid
				// for this tile
				if (available_values[j] != 0) {
					continue; // invalid value
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

/**
 * Swaps a and b
 */
/*static void swapEmptyTiles(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int a, int b) {
	struct EmptyTile temp = emptyTiles[a];
	emptyTiles[a] = emptyTiles[b];
	emptyTiles[b] = temp;
}*/

/**
 * Sort the empty tiles so that the ones with the smallest number of available
 * possibilities are first.
 */
/*static void sortEmptyTiles(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int length) {
	if (length == 0) {
		return;
	}

	int best_i, best_count, i, j;
	struct EmptyTile tile;

	// simple selection sort
	for (i = 0; i < length-1; i++) {
		best_i = i;
		best_count = emptyTiles[best_i].available_count;
		for (j = i+1; j < length; j++) {
			tile = emptyTiles[j];
			if (tile.available_count < best_count) {
				best_i = j;
				best_count = emptyTiles[best_i].available_count;
			}
		}
		if (i != best_i) {
			swapEmptyTiles(emptyTiles, i, best_i);
		}
	}
}
*/