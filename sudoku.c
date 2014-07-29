/**
 * Sudoku Board Primitives
 *
 * Author: Sunjay Varma (www.sunjay.ca)
 */
#include <stdlib.h>

#include "sudoku.h"

/**
 * Generates a completely empty, all zeros board
 *
 * Should always be freed after use.
 */
SudokuBoard* newSudokuBoard() {
	SudokuBoard* sudoku = malloc(sizeof(SudokuBoard));
	if (!sudoku) {
		return NULL;
	}
	for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
		sudoku->tiles[i] = 0;
	}
	return sudoku;
}

/**
 * Generates a completely filled, pre-solved, random board
 *
 * Should always be freed after use.
 */
SudokuBoard* solvedSudokuBoard() {
	// A presolved board to shuffle around
	short board_tiles[BOARD_SIZE*BOARD_SIZE] = {
		1, 2, 3,  4, 5, 6,  7, 8, 9,
		4, 5, 6,  7, 8, 9,  1, 2, 3,
		7, 8, 9,  1, 2, 3,  4, 5, 6,

		2, 3, 4,  5, 6, 7,  8, 9, 1,
		5, 6, 7,  8, 9, 1,  2, 3, 4,
		8, 9, 1,  2, 3, 4,  5, 6, 7,

		3, 4, 5,  6, 7, 8,  9, 1, 2,
		6, 7, 8,  9, 1, 2,  3, 4, 5,
		9, 1, 2,  3, 4, 5,  6, 7, 8
	};
	//TODO: Shuffle rows and columns before assigning
	SudokuBoard* board = malloc(sizeof(SudokuBoard));
	if (!board) {
		return NULL;
	}
	for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
		board->tiles[i] = board_tiles[i];
	}
	return board;
}

/**
 * Returns an array of size BOARD_SIZE that represents a row of the board
 *
 * Should be freed after use
 */
short* getBoardRow(SudokuBoard* board, int row_i) {
	short* row = malloc(sizeof(short)*BOARD_SIZE);
	if (!row) {
		return NULL;
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		row[i] = board->tiles[BOARD_SIZE * row_i + i];
	}
	return row;
}

/**
 * Returns an array of size BOARD_SIZE that represents a column of the board
 *
 * Should be freed after use
 */
short* getBoardColumn(SudokuBoard* board, int col_i) {
	short* col = malloc(sizeof(short)*BOARD_SIZE);
	if (!col) {
		return NULL;
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		col[i] = board->tiles[i * BOARD_SIZE + col_i];
	}
	return col;
}

/**
 * Gets one of the boxes of the board of BOX_SIZE
 * Returns an array of size BOARD_SIZE that represents the box
 *
 * Box indexes work like this for BOARD_SIZE = 9:
 * 0 1 2
 * 3 4 5
 * 6 7 8
 *
 * Should be freed after use
 */
short* getBoardBox(SudokuBoard* board, int box_i) {
	short* box = malloc(sizeof(short)*BOARD_SIZE);
	if (!box) {
		return NULL;
	}

	int box_x = box_i % BOX_SIZE, box_y = box_i / BOX_SIZE;
	int start_x = box_x * BOX_SIZE, start_y = box_y * BOX_SIZE * BOARD_SIZE;
	int start_offset = start_y + start_x;
	for (int i = 0; i < BOX_SIZE; i++) {
		for (int j = 0; j < BOX_SIZE; j++) {
			box[i * BOX_SIZE + j] = board->tiles[start_offset + i*BOARD_SIZE + j];
		}
	}
	return box;
}

/**
 * Gets the tiles in the box surrounding a given tile between
 *
 * tile_i must be between 0 and BOARD_SIZE*BOARD_SIZE-1
 *
 * Should be freed after use
 */
short* getTileBox(SudokuBoard* board, int tile_i) {
	int box_x = (tile_i % ((int)BOARD_SIZE)) / ((int)BOX_SIZE);
	int box_y = (tile_i / ((int)BOARD_SIZE)) / ((int)BOX_SIZE);
	int box_i = box_y * BOX_SIZE + box_x;
	return getBoardBox(board, box_i);
}

/**
 * Methods for converting tile index to tile coordinates
 */
short tileIndexToColumn(int tile_i) {
	return tile_i % ((int)BOARD_SIZE);
}
short tileIndexToRow(int tile_i) {
	return tile_i / ((int)BOARD_SIZE);
}

/**
 * Converts from tile coordinates (column, row) to tile index
 */
int tileCoordinatesToIndex(short col, short row) {
	return row * BOARD_SIZE + col;
}

/**
 * Gets a sorted index of all unique numbers surrounding a tile index.
 *
 * Collects numbers in the same box, column and row
 * Returns an array of BOARD_SIZE items. If a number exists arround the
 * tile, it will be in the array as is. Otherwise, it will be zero.
 * Returns NULL if the box, row and column of the given tile are empty
 *
 * Should be freed after use
 */
short* getTileSurroundings(SudokuBoard* board, int tile_i) {
	int col_i = tileIndexToColumn(tile_i);
	int row_i = tileIndexToRow(tile_i);
	short* col = getBoardColumn(board, col_i);
	short* row = getBoardRow(board, row_i);
	short* box = getTileBox(board, tile_i);

	short* numbers = calloc(BOARD_SIZE, sizeof(short));
	if (!numbers) {
		return NULL;
	}

	int found = 0; // non-zero numbers found so far
	for (int i = 0; i < BOARD_SIZE; i++) {
		if (col[i] != 0) {
			short value = col[i];
			if (numbers[value-1] == value) {
				found++;
			}
			numbers[value-1] = value;
		}
		if (row[i] != 0) {
			short value = row[i];
			if (numbers[value-1] == value) {
				found++;
			}
			numbers[value-1] = value;
		}
		if (box[i] != 0) {
			short value = box[i];
			if (numbers[value-1] == value) {
				found++;
			}
			numbers[value-1] = value;
		}
	}

	return numbers;
}
