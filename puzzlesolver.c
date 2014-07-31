#include <stdlib.h>

#include "sudoku.h"

static void simpleSolver(SudokuBoard* board);
static bool more_than_one(short value, short items[], int length);

/**
 * Sudoku solving algorithm. Returns the solved board or NULL if something
 * went horribly wrong.
 */
SudokuBoard* solveBoard(SudokuBoard* board) {
	// try the simplest algorithm first
	// this will remove any obvious solutions
	simpleSolver(board);

	if (isCompleteBoard(board)) {
		return board;
	}

	return board;
}

/**
 * Uses the simplest solving algorithm possible.
 *
 * Simply puts any obvious solutions in their places. That means
 * that if there is only one possible value for a specific position,
 * that value will be placed. No guessing.
 * Stops searching once no more obvious insertions remain.
 * Not guaranteed to produce a complete solution.
 *
 * Modifies board in place.
 */
static void simpleSolver(SudokuBoard* board) {
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
				if (more_than_one(0, available_values, BOARD_SIZE)) {
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
}

static bool more_than_one(short value, short items[], int length) {
	int n = 0;
	for (int i = 0; i < length; i++) {
		if (items[i] == value) {
			if ((++n) > 1) {
				return true;
			}
		}
	}
	return false;
}