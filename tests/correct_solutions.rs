extern crate sudoku;

use std::io::{self, Cursor};

use sudoku::*;

// Using a macro so that line numbers in error messages are better
macro_rules! test_and_check {
    ($test_set:expr => $solutions:expr) => {
        let mut test_set = read_all_boards(include_bytes!($test_set));
        let mut solutions = read_all_boards(include_bytes!($solutions));
        for i in 1.. {
            match (test_set.next(), solutions.next()) {
                (Some(mut test_board), Some(solution)) => {
                    assert!(solution.is_complete_board(), "Solution was not a solution for #{}\n{}\n", i, solution);

                    println!("Testing board #{}...", i);

                    let test_copy = test_board.clone();
                    test_board.solve()
                        .map_err(|_| format!("Unable to solve #{}.\nTest board:\n{}\n\nSolution:\n{}", i, test_copy, solution))
                        .unwrap();

                    assert_eq!(test_board, solution, "\n#{} did not result in the correct solution\nTest board:\n{}\n\nExpected Solution:\n{}\nActual Solution:\n{}\n", i, test_copy, solution, test_board);
                },
                (None, None) => break,
                _ => panic!("Test set and solutions mismatch"),
            }
        }
    };
}

#[test]
fn easy() {
    test_and_check!("../samples/easy.txt" => "../samples/easysolutions.txt");
}

#[test]
fn hard95() {
    test_and_check!("../samples/hard95.txt" => "../samples/hard95solutions.txt");
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
