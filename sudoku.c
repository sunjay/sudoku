/**
 * Sudoku
 *
 * Author: Sunjay Varma (www.sunjay.ca)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "sudoku.h"

#define COL_CHAR '|'
#define ROW_CHAR '-'
#define LETTER_A 'A'
#define LETTER_0 '0'
#define COL_PADDING 1

SudokuBoard* initSudokuBoard();
void drawSudokuBoard(SudokuBoard* game);
void getMove(unsigned short* col, unsigned short* row, unsigned short* value);

int main(int argc, char* argv[]) {
	SudokuBoard* board = initSudokuBoard();

	unsigned short move_col, move_row, value;
	while (true) {
		drawSudokuBoard(board);
		printf("\nEnter your move with the location, then the number to input: (e.g. A11)\n");
		getMove(&move_col, &move_row, &value);
		printf("%d %d %d", move_col, move_row, value);
		break;
	}
	return 0;
}

SudokuBoard* initSudokuBoard() {
	SudokuBoard* sudoku = malloc(sizeof(SudokuBoard));
	for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
		sudoku->tiles[i] = 0;
	}
	return sudoku;
}

void getMove(unsigned short* col, unsigned short* row, unsigned short* value) {
	char colLetter;
	int max_value = BOARD_SIZE;
	while (true) {
		int itemsRead = scanf("%1c%1hd%1hd", &colLetter, row, value);
		if (itemsRead == EOF) {
			exit(0);
		}
		else if (itemsRead < 3) {
			printf("Invalid input.\n");
			continue;
		}
		*col = colLetter - LETTER_A;
		unsigned int colValue = *col, rowValue = *row, number = *value;
		if (colValue < 0 || colValue > max_value || rowValue < 0 || rowValue > max_value || number < 0 || number > max_value) {
			printf("Invalid input.\n");
			continue;
		}

		// must be valid
		// fix row value
		(*row)--;
		return;
	}
}

void clearScreen() {
	system("cls");
}

void printRepeatedCharacters(char c, int repeated) {
	for (int i = 0; i < repeated; i++) {
		printf("%c", c);
	}
}

void printRowLine() {
	int tile_width = COL_PADDING*2 + 1;
	printRepeatedCharacters(ROW_CHAR, 2 + tile_width*BOARD_SIZE + BOX_SIZE+1);
	printf("\n");
}

void printLetterRow() {
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
