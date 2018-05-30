use std::{
    fmt,
    io::{self, Read},
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

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct Tile {
    /// The numeric value of the tile - value between 0 and BOARD_SIZE
    /// 0 means that the tile is empty
    value: u8,

    /// Represents every possible value that could be placed on this tile
    ///
    /// Each position is associated with a value in the range 1 to BOARD_SIZE+1
    /// true if a value is available, false if a value cannot be used on this tile
    possible_values: [bool; BOARD_SIZE],

    /// A cache of the number of true values in possibleValues
    /// Not used if value != 0
    possible_count: usize,
}

#[derive(Debug, Clone, Eq)]
pub struct Sudoku {
    /// Tiles are stored row-wise
    /// So index with tiles[row][col]
    tiles: [[Tile; BOARD_SIZE]; BOARD_SIZE],
}

impl Default for Sudoku {
    fn default() -> Self {
        let mut sudoku = Sudoku {tiles: Default::default()};
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
                write!(f, "{}", tile.value)?;
            }
            writeln!(f)?;
        }

        Ok(())
    }
}

impl PartialEq for Sudoku {
    fn eq(&self, other: &Self) -> bool {
        // Boards should be the same if their tile values are the same regardless of the other
        // metadata stored
        for (row_a, row_b) in self.tiles.iter().zip(other.tiles.iter()) {
            for (tile_a, tile_b) in row_a.iter().zip(row_b.iter()) {
                if tile_a.value != tile_b.value {
                    return false;
                }
            }
        }
        true
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
                    board.place((row_i, col_i), value - b'0');
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
        for row in &self.tiles {
            for tile in row {
                if tile.value == 0 {
                    return false;
                }
            }
        }

        true
    }

    /// Places a value on the sudoku board. Updates all related possible value
    /// caches and their counts.
    pub fn place(&mut self, (row, col): (usize, usize), value: u8) {
        assert!(value <= BOARD_SIZE as u8, "value `{}` is invalid", value);
        self.tiles[row][col].value = value;

        if value == 0 {
            return;
        }

        // The start index of this tile's box
        let box_row_start = (row / BOX_SIZE) * BOX_SIZE;
        let box_col_start = (col / BOX_SIZE) * BOX_SIZE;

        // The index at which this value will be found in all possible_values array
        let value_index = (value - 1) as usize;

        for i in 0..BOARD_SIZE {
            // Update items in the same row as the placed tile
            {
                let item = &mut self.tiles[row][i];
                if item.possible_values[value_index] {
                    // Tile was available, now it is not
                    item.possible_values[value_index] = false;
                    item.possible_count -= 1;
                }
            }

            // Update items in the same column as the placed tile
            {
                let item = &mut self.tiles[i][col];
                if item.possible_values[value_index] {
                    // Tile was available, now it is not
                    item.possible_values[value_index] = false;
                    item.possible_count -= 1;
                }
            }

            // Update items in the same box as the placed tile
            {
                let item = &mut self.tiles[box_row_start + i / BOX_SIZE][box_col_start + i % BOX_SIZE];
                if item.possible_values[value_index] {
                    // Tile was available, now it is not
                    item.possible_values[value_index] = false;
                    item.possible_count -= 1;
                }
            }
        }
    }

    /// Attempts to solve the sudoku board
    ///
    /// Only fails if all possible solutions are exhausted
    pub fn solve(&mut self) -> Result<(), SolverError> {
        // Very simple algorithm that continually fills in values with only one possible value
        // Finishes the "obvious" solutions so we don't waste any time looking for them
        self.simple_solve();

        if self.is_complete_board() {
            Ok(())
        }
        else {
            self.guess_solve()
        }
    }

    /// The first tier and simplest solving algorithm.
    ///
    /// This algorithm goes through all empty tiles and checks what values
    /// are available/valid for each tile. If there is only one possibility, that
    /// is placed on that space.
    ///
    /// Solves as much as possible. Complete solution not guaranteed.
    ///
    /// Modifies the board in place.
    fn simple_solve(&mut self) {
        loop {
            // found is used to track whether a tile was solved this time
            // through the board
            // We continue going through all the available tiles on the board
            // until we can't solve any more values with this method
            let mut found = None;

            'search: for (row_i, row) in self.tiles.iter_mut().enumerate() {
                for (col_i, tile) in row.iter_mut().enumerate() {
                    // Skip if this is not an empty tile or a tile with only a single possible value
                    // remaining
                    if tile.value != 0 || tile.possible_count != 1 {
                        continue;
                    }

                    // The only possible value is the one that is true
                    for (value, possible) in tile.possible_values.iter().enumerate() {
                        if *possible {
                            found = Some((row_i, col_i, (value + 1) as u8));
                            break 'search;
                        }
                    }
                    tile.possible_count = tile.possible_values.len();
                }
            }

            if let Some((row, col, value)) = found {
                self.place((row, col), value);
            }
            else {
                break;
            }
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
            let guess = (i + 1) as u8;

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
                if tile.value != 0 {
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
