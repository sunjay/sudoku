#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "sudoku.h"
#include "inputhandler.h"

#define LETTER_0 '0'

/**
 * Attempts to read a board from the given file pointer
 * A board is represented as 9 digits per line (separated by only '\n')
 * Exactly 9 lines are read to complete the board
 * Blank spaces are represented as 0
 *
 * Returns 0 if the operation was successful and -1 if there was a parse error
 */
int readBoard(FILE* fp, SudokuBoard* board) {
    emptySudokuBoard(board);

    char line[BOARD_SIZE];

    for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
        // EOF or line is too short or too long
        if (getnline(fp, BOARD_SIZE, line) == -1) {
            return -1;
        }

        short row[BOARD_SIZE];
        for (int i = 0; i < BOARD_SIZE; i++) {
            char c = line[i];

            if (!isdigit(c)) {
                // invalid row item
                return -1;
            }
            row[i] = c - LETTER_0;
        }

        setBoardRowValues(board, row_i, row);
    }

    return 0;
}

