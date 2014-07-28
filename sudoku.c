/**
 * Sudoku
 *
 * Author: Sunjay Varma (www.sunjay.ca)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "inputhandler.h"
#include "sudoku.h"

#define COL_CHAR '|'
#define ROW_CHAR '-'
#define LETTER_A 'A'
#define LETTER_0 '0'
#define COL_PADDING 1

Sudoku* initSudokuBoard();
void drawSudokuBoard(Sudoku* game);
void getMove(unsigned short* col, unsigned short* row, unsigned short* value);

int main(int argc, char* argv[]) {
	Sudoku* game = initSudokuBoard();

	unsigned short move_col, move_row, value;
	while (true) {
		drawSudokuBoard(game);
		printf("\nEnter your move with the location, then the number to input: (e.g. A11)\n");
		getMove(&move_col, &move_row, &value);
		printf("%d %d %d", move_col, move_row, value);
		break;
	}
	return 0;
}

Sudoku* initSudokuBoard() {
	Sudoku* sudoku = malloc(sizeof(Sudoku));
	for (int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++) {
		Board board;
		for (int j = 0; j < BOARD_SIZE*BOARD_SIZE; j++) {
			board.tiles[j] = 0;
		}
		sudoku->boards[i] = board;
	}
	return sudoku;
}

void getMove(unsigned short* col, unsigned short* row, unsigned short* value) {
	char colLetter;
	int max_value = BOARD_SIZE*BOARD_SIZE;
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
	printRepeatedCharacters(ROW_CHAR, 2 + tile_width*BOARD_SIZE*BOARD_SIZE + BOARD_SIZE+1);
	printf("\n");
}

void printLetterRow() {
	printf("  %c", COL_CHAR);
	for (int board_i = 0; board_i < BOARD_SIZE; board_i++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			printRepeatedCharacters(' ', COL_PADDING);
			printf("%c", LETTER_A + board_i * BOARD_SIZE + col);
			printRepeatedCharacters(' ', COL_PADDING);
		}
		printf("%c", COL_CHAR);
	}
	printf("\n");
}

void drawSudokuBoard(Sudoku* game) {
	// Clear the screen
	clearScreen();

	// Print column letters
	printLetterRow();

	// Print the first row separator
	printRowLine();
	for (int row = 0; row < BOARD_SIZE*BOARD_SIZE; row++) {
		printf("%d %c", row + 1, COL_CHAR);

		// print a single row from all three boards in the row
		int board_row = row / BOARD_SIZE;
		for (int board_i = 0; board_i < BOARD_SIZE; board_i++) {
			Board board = game->boards[board_row*BOARD_SIZE + board_i];
			for (int col = 0; col < BOARD_SIZE; col++) {
				printRepeatedCharacters(' ', COL_PADDING);

				int value = board.tiles[board_row * BOARD_SIZE + col];
				printf("%d", value);

				printRepeatedCharacters(' ', COL_PADDING);
			}
			printf("%c", COL_CHAR);
		}
		printf("\n");

		if ((row+1) % 3 == 0) {
			printRowLine();
		}
	}
}
