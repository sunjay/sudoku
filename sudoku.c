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
SudokuBoard* sudokuPuzzle() {
	SudokuBoard* board = malloc(sizeof(SudokuBoard));
	// A presolved board to shuffle around
	/*board->tiles = {
		1, 2, 3,  4, 5, 6,  7, 8, 9,
		4, 5, 6,  7, 8, 9,  1, 2, 3,
		7, 8, 9,  1, 2, 3,  4, 5, 6,

		2, 3, 4,  5, 6, 7,  8, 9, 1,
		5, 6, 7,  8, 9, 1,  2, 3, 4,
		8, 9, 1,  2, 3, 4,  5, 6, 7,

		3, 4, 5,  6, 7, 8,  9, 1, 2,
		6, 7, 8,  9, 1, 2,  3, 4, 5,
		9, 1, 2,  3, 4, 5,  6, 7, 8
	};*/
	//TODO: Finish this by shuffling rows and columns
	return board;
}