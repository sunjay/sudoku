use std::{
    fmt,
    io::{self, Read},
    num::NonZeroU8,
};

// The width and height of a single "box" on the board
const BOX_SIZE: usize = 3; // 3 cells per box
// The width and height of the entire board
const BOARD_SIZE: usize = 3 * BOX_SIZE; // 3 boxes

#[derive(Debug, Clone)]
pub enum SolverError {
    NoSolution,
}

#[derive(Debug)]
pub enum ReadError {
    IOError(io::Error),
    ExpectedNewline,
    InvalidDigit(char),
}

#[derive(Debug, Clone, Copy, Default, Eq)]
pub struct Tile {
    /// The numeric value of the tile - value between 1 and BOARD_SIZE (inclusive)
    /// None - tile is empty
    value: Option<NonZeroU8>,

    /// Represents every possible value that could be placed on this tile
    ///
    /// Each position is associated with a value in the range 1 to BOARD_SIZE+1
    /// true if a value is available, false if a value cannot be used on this tile
    possible_values: [bool; BOARD_SIZE],

    /// A cache of the number of true values in possibleValues
    /// Not used if tile is not empty
    possible_count: usize,
}

impl PartialEq for Tile {
    fn eq(&self, other: &Self) -> bool {
        self.value == other.value
    }
}

impl Tile {
    pub fn is_empty(&self) -> bool {
        self.value.is_none()
    }

    pub fn set(&mut self, value: NonZeroU8) {
        self.value = Some(value);
    }

    /// Marks the given value as no longer available for this tile
    pub fn mark_unavailable(&mut self, value: NonZeroU8) {
        // The index at which this value will be found in all possible_values array
        // This subtraction can never panic because NonZeroU8 >= 1
        let value_index = value.get() as usize - 1;
        if self.possible_values[value_index] {
            // Tile was available, but now it is not
            self.possible_values[value_index] = false;
            self.possible_count -= 1;
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Sudoku {
    /// Tiles are stored row-wise
    /// So index with tiles[row][col]
    tiles: [[Tile; BOARD_SIZE]; BOARD_SIZE],
    /// The number of empty tiles remaining
    empty_tiles: usize,
}

impl Default for Sudoku {
    fn default() -> Self {
        let mut sudoku = Sudoku {
            tiles: Default::default(),
            empty_tiles: BOARD_SIZE * BOARD_SIZE,
        };
        // Initialize the board to be completely empty and set all values as possible
        for row in &mut sudoku.tiles {
            for tile in row {
                for value in &mut tile.possible_values {
                    *value = true;
                }
                tile.possible_count = tile.possible_values.len();
            }
        }

        sudoku
    }
}

impl fmt::Display for Sudoku {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        for row in &self.tiles {
            for tile in row {
                write!(f, "{}", tile.value.map(|x| x.get()).unwrap_or(0))?;
            }
            writeln!(f)?;
        }

        Ok(())
    }
}

impl Sudoku {
    /// Creates a new sudoku board by reading 9 rows of 9 digits representing the values currently
    /// filled and unfilled in the grid. 0 represents an empty tile.
    pub fn read_board<R: Read>(mut reader: R) -> Result<Self, ReadError> {
        assert_eq!(BOARD_SIZE, 9, "This function assumes that BOARD_SIZE is 9");

        let mut board = Self::default();

        // Read each value + newline
        let mut buffer = [0; BOARD_SIZE + 1];
        for row_i in 0..BOARD_SIZE {
            reader.read_exact(&mut buffer).map_err(ReadError::IOError)?;
            if buffer[BOARD_SIZE] != b'\n' {
                return Err(ReadError::ExpectedNewline);
            }

            for col_i in 0..BOARD_SIZE {
                let value = buffer[col_i];
                if value.is_ascii_digit() {
                    // Only place the tile if the value is not zero
                    NonZeroU8::new(value - b'0').map(|value| {
                        board.place((row_i, col_i), value);
                    });
                }
                else {
                    return Err(ReadError::InvalidDigit(value as char));
                }
            }
        }

        Ok(board)
    }

    /// Returns true if the board is a valid sudoku board
    /// A board is invalid if there are duplicate numbers in any row, column, or box
    /// or if any of its values are out of the range 0 to BOARD_SIZE inclusive.
    pub fn is_valid(&self) -> bool {
        true // TODO
    }

    /// Returns true if the board is completely filled
    pub fn is_complete_board(&self) -> bool {
        self.empty_tiles == 0
    }

    /// Places a value on the sudoku board. Updates all related possible value
    /// caches and their counts.
    pub fn place(&mut self, (row, col): (usize, usize), value: NonZeroU8) {
        assert!(value.get() <= BOARD_SIZE as u8, "value `{}` is invalid", value);
        // Only decrement the empty tile count if the tile was previously empty
        if self.tiles[row][col].is_empty() {
            self.empty_tiles -= 1;
        }
        self.tiles[row][col].set(value);

        // The start index of this tile's box
        let box_row_start = (row / BOX_SIZE) * BOX_SIZE;
        let box_col_start = (col / BOX_SIZE) * BOX_SIZE;

        for i in 0..BOARD_SIZE {
            // Update items in the same row as the placed tile
            self.tiles[row][i].mark_unavailable(value);

            // Update items in the same column as the placed tile
            self.tiles[i][col].mark_unavailable(value);

            // Update items in the same box as the placed tile
            let box_row = box_row_start + i / BOX_SIZE;
            let box_col = box_col_start + i % BOX_SIZE;
            self.tiles[box_row][box_col].mark_unavailable(value);
        }
    }

    /// Attempts to solve the sudoku board
    ///
    /// Only fails if all possible solutions are exhausted
    pub fn solve(&mut self) -> Result<(), SolverError> {
        if self.is_complete_board() {
            Ok(())
        }
        else {
            self.guess_solve()
        }
    }

    /// Second tier of solving (intelligent brute force)
    /// Makes an intelligent guess about what position to guess on
    /// Guesses an appropriate value and attempts to solve from the first tier
    /// using the modified board
    /// If no solution is found, the initial guess is undone and the search
    /// continues
    /// Only fails if all possible solutions are exhausted.
    fn guess_solve(&mut self) -> Result<(), SolverError> {
        // Get the tile with the minimum number of possibilities
        // This is the most efficient place to start guessing because
        // if we guess wrong we will have the fewest number of alternatives
        // to search
        // On the otherhand, if we chose the tile with the maximum number
        // of possible values, we elimate more possible routes by guessing
        // right. I'm just betting that we'll guess wrong more often then
        // we guess right since there are more wrong numbers than right ones.

        let (min_row, min_col) = self.min_possible_empty_tile()?;
        let possible_values = self.tiles[min_row][min_col].possible_values;

        for i in 0..BOARD_SIZE {
            if !possible_values[i] {
                // Skip values that cannot possibly be solutions
                continue;
            }

            // The value at this index is to be used as the guess
            // Each index in possibleValues represents a value from 1-9
            // Adding 1 to the index produces the value
            // This unwrap is safe because a u8 is >= 0 and adding 1 means the value is >= 1
            let guess = NonZeroU8::new(i as u8 + 1).unwrap();

            // Make a guess
            let mut copy = self.clone();
            copy.place((min_row, min_col), guess);

            // Try to solve the board with this guess
            if copy.solve().is_ok() {
                *self = copy;
                return Ok(())
            }
        }

        Err(SolverError::NoSolution)
    }

    // Returns the tile with the minimum number of possibilities
    fn min_possible_empty_tile(&self) -> Result<(usize, usize), SolverError> {
        let mut min = Err(SolverError::NoSolution);

        for (row_i, row) in self.tiles.iter().enumerate() {
            for (col_i, tile) in row.iter().enumerate() {
                // If the tile is already filled, move on
                if !tile.is_empty() {
                    continue;
                }

                min = match min {
                    Err(_) => Ok((row_i, col_i, tile.possible_count)),
                    Ok((_, _, possible_count)) if tile.possible_count < possible_count => {
                        Ok((row_i, col_i, tile.possible_count))
                    },
                    _ => min,
                }
            }
        }

        min.map(|(row, col, _)| (row, col))
    }
}
