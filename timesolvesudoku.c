/**
 * Times the sudoku solver for every puzzle provided on stdin
 * Outputs a CSV file to stdout with the timing values
 */

// From: http://stackoverflow.com/a/3875233/551904
// Used to prevent `storage size of start isn't known` error
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sudoku.h"
#include "drawboard.h"
#include "boardparser.h"
#include "puzzlesolver.h"

#define BILLION  (1000000000L)

int main(int argc, char* argv[]) {
	printf("Puzzle Difficulty,Resolution (ns),Elapsed Time (ns)\n");
	
	int totalPuzzles = 0;
	int completed = 0;
	double averageSolveTime = 0;
	double maxTime = 0;

    struct timespec start, stop, res;

    if (clock_getres(CLOCK_MONOTONIC, &res) == -1) {
        perror("clock getres");
        exit(EXIT_FAILURE);
    }

    double resolution = res.tv_sec * BILLION + res.tv_nsec;

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

        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
            perror("clock gettime");
            exit(EXIT_FAILURE);
        }

		SudokuBoard* solved = solveBoard(board);

        if (clock_gettime(CLOCK_MONOTONIC, &stop) == -1) {
            perror("clock gettime");
            exit(EXIT_FAILURE);
        }

        double elapsedTime = (stop.tv_sec - start.tv_sec) * BILLION
                           + (stop.tv_nsec - start.tv_nsec);

		printf("%lf,%lf,%lf\n", puzzleDifficulty, resolution, elapsedTime);

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
		fprintf(stderr, "Solved %d of %d puzzles (avg %f ns, max %f ns)\n", completed, totalPuzzles, averageSolveTime, maxTime);
	}

	return EXIT_SUCCESS;
}
