/**
 * Times the sudoku solver
 * Specifically for windows which doesn't have a great time command like
 * linux
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"
#include "drawboard.h"
#include "boardparser.h"
#include "puzzlesolver.h"

int main(int argc, char* argv[]) {
	printf("Puzzle Difficulty,Ticks/sec,Resolution (ns),Elapsed Time (s)\n");
	
	int totalPuzzles = 0;
	int completed = 0;
	double averageSolveTime = 0;
	double maxTime = 0;
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
		//drawSudokuBoard(board);
		
		double puzzleDifficulty = getBoardDifficultyRating(board);

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
		//printf("\n");
		//printf("Your performance counter ticks %I64u times per second\n", freq.QuadPart);
		//printf("Resolution is %lf nanoseconds\n", resolution*1e9);
		//printf("Code took %lf sec\n", elapsedTime);
		//printf("\n");
		printf("%lf,%I64u,%lf,%lf\n", puzzleDifficulty, freq.QuadPart, resolution*1e9, elapsedTime);

		// Take the running average
		averageSolveTime = (averageSolveTime * totalPuzzles + elapsedTime)/(totalPuzzles + 1);
		totalPuzzles++;

		if (elapsedTime > maxTime) {
			maxTime = elapsedTime;
		}

		if (solved == NULL) {
			printf("No solution found.\n");
			freeSudokuBoard(board);
			continue;
		}
		else {
			completed++;
		}

		//drawSudokuBoard(solved);

		if (solved != board) {
			freeSudokuBoard(solved);
		}
		freeSudokuBoard(board);
	}

	if (totalPuzzles > 0) {
		printf("Solved %d of %d puzzles (avg %f secs, max %f secs)\n", completed, totalPuzzles, averageSolveTime, maxTime);
	}

	return 0;
}
