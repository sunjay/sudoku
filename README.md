Play & Solve Sudoku Puzzles
===========================

Author Information
------------------
Name: Sunjay Varma<br>
Website: http://www.sunjay.ca

Introduction
------------
This is a sudoku implementation in C. You can both play sudoku (TODO) and
solve sudoku puzzles.

Usage
-----
### Solve Sudoku Puzzles ###
Compile the project using make:

    $ make solvesudoku

Run it with some input:

    $ solvesudoku < input.txt

Or type in your input manually

    $ solvesudoku
    200080300
    060070084
    030500209
    000105408
    000000000
    402706000
    301007040
    720040060
    004010003

The solver expects input in the following format:

* One row per line, numbers only.
* Exactly 9 items per row.
* 0 marks an empty space.

You can pass in as many boards as you want. Separate boards must follow 
each other in the input one right after another.

Once the board is solved, it is printed to standard output.

### Play Sudoku ###
Compile the project using make:

    $ make playsudoku

TODO: This feature is still incomplete. Once it works I'll let you know.

### Format Sudoku Puzzles ###
This simple program can be used to format the output of the solving program (or
any sudoku puzzle). Just give it any puzzle in the same format as for the solver
and it wil format it nicely.

This simply formats and prints a board:

	$ formatsudoku < input.txt
	  | A  B  C | D  E  F | G  H  I |
	---------------------------------
	1 | 4  0  0 | 0  0  0 | 8  0  5 |
	2 | 0  3  0 | 0  0  0 | 0  0  0 |
	3 | 0  0  0 | 7  0  0 | 0  0  0 |
	---------------------------------
	4 | 0  2  0 | 0  0  0 | 0  6  0 |
	5 | 0  0  0 | 0  8  0 | 4  0  0 |
	6 | 0  0  0 | 0  1  0 | 0  0  0 |
	---------------------------------
	7 | 0  0  0 | 6  0  3 | 0  7  0 |
	8 | 5  0  0 | 2  0  0 | 0  0  0 |
	9 | 1  0  4 | 0  0  0 | 0  0  0 |

This solves the board and then formats it:

	$ solvesudoku < input.txt | formatsudoku
	  | A  B  C | D  E  F | G  H  I |
	---------------------------------
	1 | 4  1  7 | 3  6  9 | 8  2  5 |
	2 | 6  3  2 | 1  5  8 | 9  4  7 |
	3 | 9  5  8 | 7  2  4 | 3  1  6 |
	---------------------------------
	4 | 8  2  5 | 4  3  7 | 1  6  9 |
	5 | 7  9  1 | 5  8  6 | 4  3  2 |
	6 | 3  4  6 | 9  1  2 | 7  5  8 |
	---------------------------------
	7 | 2  8  9 | 6  4  3 | 5  7  1 |
	8 | 5  7  3 | 2  9  1 | 6  8  4 |
	9 | 1  6  4 | 8  7  5 | 2  9  3 |
	---------------------------------

File Structure & Project Details
--------------------------------

TODO.