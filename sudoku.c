/**
 * Sudoku Board Primitives
 *
 * Author: Sunjay Varma (www.sunjay.ca)
 */
#include <stdio.h>
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
	for (int i = 0; i < BOARD_SIZE; i++) {
		sudoku->tiles[i] = calloc(BOARD_SIZE, sizeof(short));
		if (!sudoku->tiles[i]) {
			return NULL;
		}
	}
	return sudoku;
}

/**
 * Generates a valid, completely filled, pre-solved, random board
 *
 * Currently only supports b
 *
 * Should always be freed after use.
 */
SudokuBoard* solvedSudokuBoard() {
	// A presolved board to shuffle around
	SudokuBoard* board = newSudokuBoard();
	if (board == NULL) {
		return NULL;
	}

	short row0[BOARD_SIZE] = {1, 2, 3,  4, 5, 6,  7, 8, 9};
	short row1[BOARD_SIZE] = {4, 5, 6,  7, 8, 9,  1, 2, 3};
	short row2[BOARD_SIZE] = {7, 8, 9,  1, 2, 3,  4, 5, 6};

	short row3[BOARD_SIZE] = {2, 3, 4,  5, 6, 7,  8, 9, 1};
	short row4[BOARD_SIZE] = {5, 6, 7,  8, 9, 1,  2, 3, 4};
	short row5[BOARD_SIZE] = {8, 9, 1,  2, 3, 4,  5, 6, 7};

	short row6[BOARD_SIZE] = {3, 4, 5,  6, 7, 8,  9, 1, 2};
	short row7[BOARD_SIZE] = {6, 7, 8,  9, 1, 2,  3, 4, 5};
	short row8[BOARD_SIZE] = {9, 1, 2,  3, 4, 5,  6, 7, 8};

	//TODO: Shuffle rows and columns randomly
	setBoardRow(board, 0, row0);
	setBoardRow(board, 1, row1);
	setBoardRow(board, 2, row2);
	setBoardRow(board, 3, row3);
	setBoardRow(board, 4, row4);
	setBoardRow(board, 5, row5);
	setBoardRow(board, 6, row6);
	setBoardRow(board, 7, row7);
	setBoardRow(board, 8, row8);
	return board;
}

/**
 * Sets all items in a single board row to items
 *
 * items must be exactly BOARD_SIZE size
 */
void setBoardRow(SudokuBoard* board, int row_i, short* items) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		board->tiles[row_i][i] = items[i];
	}
}

/**
 * Returns an array of size BOARD_SIZE that represents a row of the board
 *
 * Should be freed after use
 */
short* getBoardRow(SudokuBoard* board, int row_i) {
	return board->tiles[row_i];
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
		col[i] = board->tiles[i][col_i];
	}
	return col;
}

/**
 * Gets one of the boxes of the board of BOX_SIZE
 * Returns an array of array pointers that represents the box
 *
 * Box indexes work like this for BOARD_SIZE = 9:
 * 0 1 2
 * 3 4 5
 * 6 7 8
 *
 * Should be freed after use
 */
short** getBoardBox(SudokuBoard* board, int box_i) {
	short** box = malloc(sizeof(short*)*BOX_SIZE);
	if (!box) {
		return NULL;
	}

	int y_offset = (box_i / ((int)BOX_SIZE)) * BOX_SIZE;
	int x_offset = (box_i % ((int)BOX_SIZE)) * BOX_SIZE;
	for (int i = 0; i < BOX_SIZE; i++) {
		box[i] = malloc(sizeof(short)*BOX_SIZE);
		for (int j = 0; j < BOX_SIZE; j++) {
			box[i][j] = board->tiles[y_offset + i][x_offset + j];
		}
	}
	return box;
}

/**
 * Converts tile column/row position to box index
 */
int coordinatesToBoxIndex(int col_i, int row_i) {
	return (row_i / ((int)BOX_SIZE))*BOX_SIZE + col_i / ((int)BOX_SIZE);
}

/**
 * Gets the tiles in the box surrounding a given tile between
 *
 * col and row are the row and column index of the tile
 *
 * Should be freed after use
 */
short** getTileBox(SudokuBoard* board, int col_i, int row_i) {
	int box_i = coordinatesToBoxIndex(col_i, row_i);
	return getBoardBox(board, box_i);
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
short* getTileSurroundings(SudokuBoard* board, int col_i, int row_i) {
	short* col = getBoardColumn(board, col_i);
	short* row = getBoardRow(board, row_i);
	short** box = getTileBox(board, col_i, row_i);

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
		if (box[i / BOX_SIZE][i % BOX_SIZE] != 0) {
			short value = box[i / BOX_SIZE][i % BOX_SIZE];
			if (numbers[value-1] == value) {
				found++;
			}
			numbers[value-1] = value;
		}
	}

	return numbers;
}

/**
 * Checks if a value is in the given array
 */
bool arrayContains(short item, short* array, int size) {
	// linear search
	for (int i = 0; i < size; i++) {
		if (array[i] == item) {
			return true;
		}
	}
	return false;
}

/**
 * Validates a single tile by checking if there are repetitions in the
 * tile's row, column or box. This is much faster than checking the entire
 * board after every move.
 */
bool validateTile(SudokuBoard* board, int col_i, int row_i) {
	short* row = getBoardRow(board, row_i);
	short* col = getBoardColumn(board, col_i);
	short* box = getTileBox(board, col_i, row_i);

	// go through all possible values and check if each value is in any
	// of these arrays more than once
	for (short n = 1; n <= BOARD_SIZE; n++) {
		//TODO: Deal with values in the same position?
	}
}

/**
 * Validates whether the board is correct
 *
 * A board is incorrect if it has any repetitions
 */
bool validateBoard(SudokuBoard* board) {
	// A cache of the boxes so we don't need to retrieve them over and over again
	short* box_cache[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		box_cache[i] = getBoardBox(board, i);
	}

	short* columns_cache[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		columns_cache[i] = getBoardColumn(board, i);
	}

	short* rows_cache[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++) {
		rows_cache[i] = getBoardRow(board, i);
	}

	// go through and validate each tile
	for (int i = 0; i < BOARD_SIZE; i++) {

	}
}