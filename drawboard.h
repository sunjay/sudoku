#ifndef __DRAWBOARD_DEFS
#define __DRAWBOARD_DEFS

#include "sudoku.h"

#define LETTER_A 'A'

void clearScreen();
void drawSudokuBoard(SudokuBoard* game);
void drawSudokuBoardSimple(SudokuBoard* board);

#endif