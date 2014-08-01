#include <stdlib.h>

#include "sudoku.h"

struct EmptyTile {
	int row;
	int col;
	short* available_values;
	int available_count;
};

static int count(short value, short items[], int length);
static void sortEmptyTiles(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int length);

/**
 * Sudoku solving algorithm. Returns the solved board or NULL if something
 * went horribly wrong.
 */
SudokuBoard* solveBoard(SudokuBoard* board) {
	struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE];
	int empty_i;
	bool foundValue;
	while (true) {
		// These variables need to be inside the loop so that they
		// reset every time the algorithm goes back through the entire
		// board
		empty_i = 0;
		foundValue = false;
		for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
			short* row = board->tiles[row_i];
			for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
				short value = row[col_i];
				if (value != 0) {
					continue;
				}

				// empty tile
				short* available_values = getTileSurroundings(board, col_i, row_i);
				int available_count = count(0, available_values, BOARD_SIZE);
				if (available_count != 1) {
					// Save the empty tile for further propagation
					struct EmptyTile tile;
					tile.row = row_i;
					tile.col = col_i;
					tile.available_values = available_values;
					tile.available_count = available_count;
					emptyTiles[empty_i++] = tile;

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

	if (isCompleteBoard(board)) {
		return board;
	}

	// go through remaining emptyTiles and make guesses
	if (empty_i == 0) {
		// no empty tiles
		return NULL;
	}

	sortEmptyTiles(emptyTiles, empty_i);

	struct EmptyTile tile;
	for (int i = 0; i < empty_i; i++) {
		tile = emptyTiles[i];

		short* available_values = tile.available_values;
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
			copy->tiles[tile.row][tile.col] = guess;

			// Try to solve the board
			SudokuBoard* solved = solveBoard(copy);

			// If there's a solution, return it
			if (solved != NULL) {
				if (solved != board) {
					free(board);
				}
				return solved;
			}

			free(solved);
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
static void swapEmptyTiles(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int a, int b) {
	struct EmptyTile temp = emptyTiles[a];
	emptyTiles[a] = emptyTiles[b];
	emptyTiles[b] = temp;
}

/**
 * Sort the empty tiles so that the ones with the smallest number of available
 * possibilities are first.
 */
static void sortEmptyTiles(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int length) {
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
