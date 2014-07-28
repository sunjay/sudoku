// The width/height of a single board and also the big board
#define BOARD_SIZE 3

typedef struct {
	short tiles[BOARD_SIZE*BOARD_SIZE];
} Board;

typedef struct {
	Board boards[BOARD_SIZE*BOARD_SIZE];
} Sudoku;