extern crate sudoku;

use std::io::stdin;

use sudoku::*;

fn main() {
    let stdin = stdin();
    while let Ok(board) = Sudoku::read_board(stdin.lock()) {
        if !board.is_valid() {
            eprintln!("Invalid board.");
            continue;
        }

        match board.solve() {
            Ok(board) => print!("{}", board),
            Err(SolverError::NoSolution) => eprintln!("No solution found."),
        }
    }
}
