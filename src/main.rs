mod sudoku;

use sudoku::Sudoku;

fn main() {
    let game = Sudoku::default();
    println!("{}", game);
}
