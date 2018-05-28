extern crate sudoku;

use std::io::stdin;

use sudoku::*;

fn main() {
    let stdin = stdin();
    while let Ok(mut board) = Sudoku::read_board(stdin.lock()) {
        if !board.is_valid() {
            eprintln!("Invalid board.");
            continue;
        }

        if let Err(SolverError::NoSolution) = board.solve() {
            eprintln!("No solution found.");
            continue;
        }

        println!("{}", board);
    }
}
