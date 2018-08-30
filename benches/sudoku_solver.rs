#[macro_use]
extern crate criterion;

extern crate sudoku;

use std::io::{self, Cursor};

use criterion::{Criterion};
use sudoku::{Sudoku, ReadError};

criterion_main!(solver);

macro_rules! solver_bench_group {
    ($($name:ident, $file:expr;)*) => {
        criterion_group! {
            name = solver;
            config = Criterion::default().sample_size(10);
            targets = $($name),*
        }

        $(
            fn $name(c: &mut Criterion) {
                // We only time the solving process here and do not check whether the solution is correct.
                // Correctness is checked in the test suite.
                let boards = read_all_boards(include_bytes!($file));
                c.bench_function(stringify!($name), move |b| b.iter_with_setup(
                    || {
                        boards.clone()
                    },
                    |boards| {
                        for (i, mut board) in boards.into_iter().enumerate() {
                            board.clone().solve()
                                .map_err(|_| format!("Unable to solve #{}", i))
                                .unwrap();
                        }
                    }
                ));
            }
        )*
    };
}

solver_bench_group! {
    easy, "../samples/easy.txt";
    hard95, "../samples/hard95.txt";
    longest10, "../samples/longest10.txt";
}

/// Read all the boards in a given string into Sudoku structs
///
/// We do this all at once instead of lazily because we don't want any unnecessary overhead
/// in the benchmark.
fn read_all_boards(data: &'static [u8]) -> Vec<Sudoku> {
    let mut boards = Vec::new();

    let mut reader = Cursor::new(data);
    loop {
        let board = match Sudoku::read_board(&mut reader) {
            Ok(board) => Ok(board),
            Err(ReadError::IOError(ref err)) if err.kind() == io::ErrorKind::UnexpectedEof => break,
            Err(err) => Err(err),
        }.expect("Error reading sudoku board");

        boards.push(board);
    }

    boards
}
