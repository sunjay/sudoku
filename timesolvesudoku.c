/**
 * Times the sudoku solver
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "drawboard.h"
#include "boardparser.h"
#include "puzzlesolver.h"

int main(int argc, char* argv[]) {
	while (true) {
		SudokuBoard* board = readBoard(stdin);
		if (board == NULL) {
			break;
		}

		if (!isValidBoard(board)) {
			printf("Invalid board.\n");
			freeSudokuBoard(board);
			continue;
		}

		drawSudokuBoard(board);

		LARGE_INTEGER freq;
		LARGE_INTEGER t0, tF, tDiff;
		double elapsedTime;
		double resolution;

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&t0);

		SudokuBoard* solved = solveBoard(board);

		QueryPerformanceCounter(&tF);
		tDiff.QuadPart = tF.QuadPart - t0.QuadPart;
		elapsedTime = tDiff.QuadPart / (double) freq.QuadPart;
		resolution = 1.0 / (double) freq.QuadPart;
		printf("Your performance counter ticks %I64u times per second\n", freq.QuadPart);
		printf("Resolution is %lf nanoseconds\n", resolution*1e9);
		printf("Code under test took %lf sec\n", elapsedTime);

		if (solved == NULL) {
			printf("No solution found.\n");
			freeSudokuBoard(board);
			continue;
		}
		drawSudokuBoard(solved);

		if (solved != board) {
			freeSudokuBoard(solved);
		}
		freeSudokuBoard(board);
	}

	return 0;
}