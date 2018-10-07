#[macro_use]
extern crate criterion;

extern crate sudoku;

use std::io::{self, Cursor};
use std::fmt;

use criterion::{Criterion, Bencher};
use sudoku::{Sudoku, ReadError};

/// Special input type so the benchmark is displayed nicely
#[derive(Clone)]
struct BenchInput(Vec<(usize, Sudoku)>);

impl fmt::Debug for BenchInput {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        // Assumption: The difference between all consecutive items must be 1
        assert!(self.0.windows(2).map(|a| a[1].0 - a[0].0).all(|diff| diff == 1));
        write!(f, "Puzzle {} to {}", self.0.first().unwrap().0 + 1, self.0.last().unwrap().0 + 1)
    }
}

impl<'a> From<&'a [(usize, Sudoku)]> for BenchInput {
    fn from(items: &'a [(usize, Sudoku)]) -> Self {
        BenchInput(items.into())
    }
}

impl BenchInput {
    fn iter_mut(&mut self) -> impl Iterator<Item=&mut (usize, Sudoku)> {
        self.0.iter_mut()
    }
}

criterion_main!(solver);

macro_rules! solver_bench_group {
    ($($name:ident, $file:expr;)*) => {
        criterion_group! {
            name = solver;
            config = Criterion::default().sample_size(10).noise_threshold(0.025);
            targets = $($name),*
        }

        $(
            fn $name(c: &mut Criterion) {
                // We only time the solving process here and do not check whether the solution is correct.
                // Correctness is checked in the test suite.
                let boards: Vec<_> = read_all_boards(include_bytes!($file)).enumerate().collect();
                let boards: Vec<BenchInput> = boards.chunks(20).map(Into::into).collect();
                c.bench_function_over_inputs(concat!(stringify!($name), "_solve"),
                    |b: &mut Bencher, boards| b.iter_with_setup(
                        || boards.clone(),
                        |mut boards: BenchInput| boards.iter_mut()
                            .map(|(i, board)| {
                                board.solve().map_err(|_| format!("Unable to solve #{}", i))
                            })
                            .collect::<Result<Vec<_>, _>>()
                            .unwrap(),
                    ),
                    boards,
                );
            }
        )*
    };
}

solver_bench_group! {
    easy, "../samples/easy.txt";
    hard95, "../samples/hard95.txt";
    longest10, "../samples/longest10.txt";
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
