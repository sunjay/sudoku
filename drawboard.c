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

static void clearScreen() {
	system("cls");
}

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
	// Clear the screen
	clearScreen();

	// Print column letters
	printLetterRow();

	// Print the first row separator
	printRowLine();
	for (int row = 0; row < BOARD_SIZE; row++) {
		// Row number
		printf("%d ", row + 1, COL_CHAR);

		for (int col = 0; col < BOARD_SIZE; col++) {
			// board column separator
			if (col % 3 == 0) {
				printf("%c", COL_CHAR);
			}

			// print the value itself
			printRepeatedCharacters(' ', COL_PADDING);

			int value = board->tiles[row * BOARD_SIZE + col];
			printf("%d", value);

			printRepeatedCharacters(' ', COL_PADDING);
		}
		printf("%c\n", COL_CHAR);

		// board row separator
		if ((row+1) % 3 == 0) {
			printRowLine();
		}
	}
}
