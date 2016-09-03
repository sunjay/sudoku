/**
 * A sudoku solving program.
 *
 * Reads boards as number rows from stdin
 * Rows should be BOARD_SIZE
 * Solves as many boards as provided on stdin until EOF
 * Use 0 to mark an empty tile
 */

#include <stdio.h>

#include "sudoku.h"
#include "drawboard.h"
#include "boardparser.h"

int main(int argc, char* argv[]) {
    SudokuBoard board;
    while (true) {
        readBoard(stdin, &board);

        drawSudokuBoard(&board);
    }
    return 0;
}
