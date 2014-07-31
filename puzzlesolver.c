#include <stdio.h>
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
	int empty_i = 0;
	bool foundValue;
	while (true) {
		foundValue = false;
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				short value = board->tiles[i][j];
				if (value != 0) {
					continue;
				}

				// empty tile
				short* available_values = getTileSurroundings(board, j, i);
				int available_count = count(0, available_values, BOARD_SIZE);
				if (available_count > 1) {
					// Save the empty tile for further propagation
					struct EmptyTile tile;
					tile.row = i;
					tile.col = j;
					tile.available_values = available_values;
					tile.available_count = available_count;
					emptyTiles[empty_i++] = tile;
					continue; // more than one possible value to place here
				}

				// The only possible value
				short only_value = 0;
				for (int i = 0; i < BOARD_SIZE; i++) {
					if (available_values[i] == 0) {
						only_value = i + 1;
						break;
					}
				}

				foundValue = true;
				board->tiles[i][j] = only_value;
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
	for (int i = 0; i < empty_i; i++) {
		struct EmptyTile tile = emptyTiles[i];
		printf("%d %d %d\n", tile.col, tile.row, tile.available_count);
	}

	return board;
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

	// simple selection sort
	for (int i = 0; i < length; i++) {
		int best_i = 0;
		for (int j = i+1; j < BOARD_SIZE*BOARD_SIZE; j++) {
			struct EmptyTile tile = emptyTiles[j];
			if (tile.available_count < emptyTiles[best_i].available_count) {
				best_i = j;
			}
		}
		if (i != best_i) {
			swapEmptyTiles(emptyTiles, i, best_i);
		}
	}
}