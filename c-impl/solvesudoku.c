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
#include "puzzlesolver.h"

int main(int argc, char* argv[]) {
    SudokuBoard board;

    while (true) {
        if (readBoard(stdin, &board) == -1) {
            break;
        }

        if (!isValidBoard(&board)) {
            printf("Invalid board.\n");
            continue;
        }

        if (solveBoard(&board) == -1) {
            printf("No solution found.\n");
            continue;
        }

        drawSudokuBoardSimple(&board);
    }

    return 0;
}
