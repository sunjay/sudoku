#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sudoku.h"
#include "inputhandler.h"

#define LETTER_0 '0'

SudokuBoard* readBoard(FILE* fp) {
    SudokuBoard* board = newSudokuBoard();

    int row_i = 0;
    char* line;
    while (true) {
        line = getline(fp);
        if (line == NULL) { // EOF + Not enough rows given
            return NULL;
        }
        short row[BOARD_SIZE];
        int found = 0;
        for (int i = 0, n = strlen(line); i < n; i++) {
            char c = line[i];
            if (c == EOL) {
                break;
            }
            if (!isdigit(c) || found > BOARD_SIZE) {
                printf("invalid row item");
                free(line);
                return NULL;
            }
            row[found++] = c - LETTER_0;
        }

        free(line);
        if (found != BOARD_SIZE) {
            printf("invalid row size");
            return NULL;
        }

        setBoardRowValues(board, row_i++, row);
        if (row_i >= BOARD_SIZE) {
            break;
        }
    }
    return board;
}