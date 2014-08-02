#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

struct EmptyTile {
	int row;
	int col;
	short* available_values;
	int available_count;
};

struct EmptyTilesArray {
	struct EmptyTile tiles[BOARD_SIZE*BOARD_SIZE];
	int length;
};

static int count(short value, short items[], int length);
static void sortEmptyTiles(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int length);
static void freeEmptyTileAvailableValues(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int length);
static struct EmptyTilesArray* simpleSolver(SudokuBoard* board);
static struct EmptyTilesArray* smartSolver(SudokuBoard* board, struct EmptyTilesArray* emptyTilesManager);
static SudokuBoard* guessSolver(SudokuBoard* board, struct EmptyTilesArray* emptyTilesManager);

/**
 * Sudoku solving algorithm. Returns the solved board or NULL if something
 * went horribly wrong.
 */
SudokuBoard* solveBoard(SudokuBoard* board) {
	// First algorithm
	struct EmptyTilesArray* emptyTilesManager = simpleSolver(board);
	if (emptyTilesManager == NULL) {
		return NULL;
	}

	if (isCompleteBoard(board)) {
		return board;
	}

	// Second algorithm
	emptyTilesManager = smartSolver(board, emptyTilesManager);
	if (emptyTilesManager == NULL) {
		return NULL;
	}

	if (isCompleteBoard(board)) {
		return board;
	}

	// Last resort brute-force guess
	return guessSolver(board, emptyTilesManager);
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
static struct EmptyTilesArray* simpleSolver(SudokuBoard* board) {
	struct EmptyTilesArray* emptyTilesManager = malloc(sizeof(struct EmptyTilesArray));
	if (!emptyTilesManager) {
		return NULL;
	}

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
				// Is this an empty tile?
				if (value != 0) {
					continue;
				}

				short* available_values = getTileSurroundings(board, col_i, row_i);
				int available_count = count(0, available_values, BOARD_SIZE);
				if (available_count != 1) {
					// Save the empty tile for further propagation
					struct EmptyTile tile;
					tile.row = row_i;
					tile.col = col_i;
					tile.available_values = available_values;
					tile.available_count = available_count;
					emptyTilesManager->tiles[empty_i++] = tile;

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

	emptyTilesManager->length = empty_i;
	return emptyTilesManager;
}

/**
 * Attempts to solve the board based on patterns. Only makes complete
 * 100% confidence choices. (No guesses)
 *
 * This algoirthm uses the surrounding tiles instead of just the ones
 * in the same row.
 */
static struct EmptyTilesArray* smartSolver(SudokuBoard* board, struct EmptyTilesArray* initialEmptyTiles) {
	struct EmptyTilesArray* emptyTilesManager = malloc(sizeof(struct EmptyTilesArray));
	if (!emptyTilesManager) {
		return NULL;
	}

	struct EmptyTile* emptyTiles = initialEmptyTiles->tiles;
	int num_empty_tiles = initialEmptyTiles->length;

	int empty_i = 0;
	// go through empty tiles
	for (int i = 0; i < num_empty_tiles; i++) {
		struct EmptyTile emptyTile = emptyTiles[i];

		// Uses the surrounding rows and columns (in the same box) to
		// narrow down what this empty tile could be
		int box_start_row = (emptyTile.row / BOX_SIZE) * BOX_SIZE;
		int box_start_col = (emptyTile.col / BOX_SIZE) * BOX_SIZE;

		// Go through each possible value
		for (int value = 1; value <= BOARD_SIZE; value++) {
			;
		}

		// If this tile could not be filled, save it in the new empty tile manager
		emptyTilesManager->tiles[empty_i++] = emptyTile;
	}

	emptyTilesManager->length = empty_i;
	return emptyTilesManager;
}

/**
 * Attempts to guess on each empty tile based on the values available
 */
static SudokuBoard* guessSolver(SudokuBoard* board, struct EmptyTilesArray* emptyTilesManager) {
	struct EmptyTile* emptyTiles = emptyTilesManager->tiles;
	int num_empty_tiles = emptyTilesManager->length;

	// go through remaining emptyTiles and make guesses
	if (num_empty_tiles == 0) {
		// no empty tiles and no solution
		return NULL;
	}

	sortEmptyTiles(emptyTiles, num_empty_tiles);

	struct EmptyTile tile;
	for (int i = 0; i < num_empty_tiles; i++) {
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

			system("cls");
			printf("Guessing %d at (%d, %d) Empty: %2d ", guess, tile.col, tile.row, num_empty_tiles);
			for (int o = 0; o < num_empty_tiles; o++) {
				printf("X");
			}
			printf("\n");
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
				if (solved != board) {
					free(board);
				}
				freeEmptyTileAvailableValues(emptyTiles, num_empty_tiles);
				free(emptyTilesManager);
				return solved;
			}

			free(solved);
		}
	}

	freeEmptyTileAvailableValues(emptyTiles, num_empty_tiles);
	free(emptyTilesManager);
	return NULL;
}

static void freeEmptyTileAvailableValues(struct EmptyTile emptyTiles[BOARD_SIZE*BOARD_SIZE], int length) {
	for (int i = 0; i < length; i++) {
		free(emptyTiles[i].available_values);
	}
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
