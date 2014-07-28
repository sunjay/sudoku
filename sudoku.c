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
#include "drawboard.h"

SudokuBoard* initSudokuBoard();
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