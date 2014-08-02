/**
 * A sudoku solving program.
 *
 * Reads boards as number rows from stdin
 * Rows should be BOARD_SIZE
 * Solves as many boards as provided on stdin until EOF
 * Use 0 to mark an empty tile
 */

#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "drawboard.h"
#include "boardparser.h"

int main(int argc, char* argv[]) {
	while (true) {
		SudokuBoard* board = readBoard(stdin);
		if (board == NULL) {
			break;
		}

		drawSudokuBoard(board);
		freeSudokuBoard(board);
	}
	return 0;
}