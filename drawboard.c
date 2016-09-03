/**
 * Functions and logic for drawing the Sudoku board
 */

#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "drawboard.h"

#define COL_CHAR '|'
#define ROW_CHAR '-'
#define LETTER_0 '0'
#define COL_PADDING 1

static void printRepeatedCharacters(char c, int repeated) {
    for (int i = 0; i < repeated; i++) {
        printf("%c", c);
    }
}

static void printRowLine() {
    int tile_width = COL_PADDING*2 + 1;
    printRepeatedCharacters(ROW_CHAR, 2 + tile_width*BOARD_SIZE + BOX_SIZE+1);
    printf("\n");
}

static void printLetterRow() {
    printf("  "); // left padding to account for row number
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (i % 3 == 0) {
            printf("%c", COL_CHAR);
        }

        printRepeatedCharacters(' ', COL_PADDING);
        printf("%c", LETTER_A + i);
        printRepeatedCharacters(' ', COL_PADDING);
    }
    printf("%c\n", COL_CHAR);
}

void drawSudokuBoard(SudokuBoard* board) {
    // Print column letters
    printLetterRow();

    // Print the first row separator
    printRowLine();
    for (int row_i = 0; row_i < BOARD_SIZE; row_i++) {
        // Row number
        printf("%d ", row_i + 1);

        Tile row[BOARD_SIZE];
        getBoardRow(board, row_i, row);
        for (int col_i = 0; col_i < BOARD_SIZE; col_i++) {
            // board column separator
            if (col_i % 3 == 0) {
                printf("%c", COL_CHAR);
            }

            // print the value itself
            printRepeatedCharacters(' ', COL_PADDING);

            int value = row[col_i].value;
            printf("%d", value);

            printRepeatedCharacters(' ', COL_PADDING);
        }
        printf("%c\n", COL_CHAR);

        // board row separator
        if ((row_i+1) % 3 == 0) {
            printRowLine();
        }
    }
}

/**
 * Just simply outputs each row on its own line
 */
void drawSudokuBoardSimple(SudokuBoard* board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        Tile row[BOARD_SIZE];
        getBoardRow(board, i, row);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%d", row[j].value);
        }
        printf("\n");
    }
}
