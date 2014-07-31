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
#include <math.h>

#include "sudoku.h"
#include "inputhandler.h"

int main(int argc, char* argv[]) {
	char* line;
	while (true) {
		line = getline();
		if (line == NULL) { // EOF
			break;
		}
		// TODO: Write function to parse board
	}

	return 0;
}