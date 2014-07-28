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
SudokuBoard* solvedSudokuPuzzle() {
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
	for (int i = 0; i < BOARD_SIZE; i++) {
		col[i] = board->tiles[i * BOARD_SIZE + col_i];
	}
	return col;
}

