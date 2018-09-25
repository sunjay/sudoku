//! NOTE: This should technically be in the bin directory but default-run in Cargo.toml is not
//!  stable yet.
//!
//! Run with `cargo test -q -- --nocapture combined21886 --quiet`
//! And then delete any extra output at the beginning and end + any line starting with "test"

extern crate sudoku;

use std::io::{self, Cursor};
use std::time::{Instant};

use sudoku::*;

#[test]
fn combined21886() {
    let test_set = read_all_boards(include_bytes!("../samples/combined_21886.txt"));
    println!("Puzzle Difficulty,Elapsed Time (ns)");
    for (i, mut board) in test_set.enumerate() {
        let difficulty = board.difficulty();
        let start = Instant::now();

        board.solve()
            .map_err(|_| format!("Unable to solve #{}.", i))
            .unwrap();

        let elapsed = start.elapsed();
        let nanos = elapsed.as_secs() as u128 * 1_000_000_000 + elapsed.subsec_nanos() as u128;
        println!("{:.3},{}", difficulty, nanos);
    }
}

fn read_all_boards(data: &'static [u8]) -> impl Iterator<Item=Sudoku> {
    // FIXME: Eventually when Rust supports `yield`, this could be rewritten without the extra struct
    let reader = Cursor::new(data);
    BoardReader {reader}
}

struct BoardReader<'a> {
    reader: Cursor<&'a [u8]>,
}

impl<'a> Iterator for BoardReader<'a> {
    type Item = Sudoku;

    fn next(&mut self) -> Option<Self::Item> {
        match Sudoku::read_board(&mut self.reader) {
            Ok(board) => Ok(Some(board)),
            Err(ReadError::IOError(ref err)) if err.kind() == io::ErrorKind::UnexpectedEof => Ok(None),
            Err(err) => Err(err),
        }.expect("Error reading sudoku board")
    }
}
