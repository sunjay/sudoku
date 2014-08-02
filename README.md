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

File Structure & Project Details
--------------------------------

TODO.