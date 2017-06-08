mod sudoku;

use std::io;
use std::io::BufRead;

use sudoku::{Sudoku, BOARD_SIZE};

fn main() {
    let mut game = Sudoku::default();

    let stdin = io::stdin();
    for (i, line) in stdin.lock().lines().enumerate() {
        let line = line.expect("Could not read input");
        for (j, c) in line.chars().enumerate() {
            match c {
                '0' => continue,
                '1' ... '9' => game.place((i % BOARD_SIZE, j % BOARD_SIZE),
                    c.to_digit(10).unwrap() as usize),
                _ => panic!("Invalid character, must be in range 0 to 9"),
            }
        }

        if (i + 1) % BOARD_SIZE == 0 {
            println!("{:#?}", game);
            println!("{}", game);
            game.solve();
            println!("{}", game);
            game = Sudoku::default();
        }
    }
}
