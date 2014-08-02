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
#include "puzzlesolver.h"

int main(int argc, char* argv[]) {
	while (true) {
		SudokuBoard* board = readBoard(stdin);
		if (board == NULL) {
			break;
		}

		if (!isValidBoard(board)) {
			printf("Invalid board.\n");
			freeSudokuBoard(board);
			continue;
		}

		SudokuBoard* solved = solveBoard(board);

		if (solved == NULL) {
			printf("No solution found.\n");
			freeSudokuBoard(board);
			continue;
		}
		drawSudokuBoardSimple(solved);

		if (solved != board) {
			freeSudokuBoard(solved);
		}
		freeSudokuBoard(board);
	}

	return 0;
}