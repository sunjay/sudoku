use std::fmt;
use std::iter::repeat;

pub const BOX_SIZE: usize = 3;
pub const BOARD_SIZE: usize = BOX_SIZE * BOX_SIZE;

#[derive(Debug, Clone, Copy)]
struct Tile {
    pub value: Option<usize>,
    // Each position in this array represents one of the numbers from 1 to BOARD_SIZE
    // If true, that number is still a valid move for this tile
    // Otherwise, that number cannot be placed in this tile
    valid_values: [bool; BOARD_SIZE],
    // A cache of the number of leftover moves in valid_moves
    remaining_values: usize,
}

impl Default for Tile {
    fn default() -> Tile {
        Tile {
            value: None,
            valid_values: [true; BOARD_SIZE],
            remaining_values: BOARD_SIZE,
        }
    }
}

impl fmt::Display for Tile {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.write_str(&self.value.map(|v| v.to_string()).unwrap_or(" ".to_owned()))
    }
}

impl Tile {
    /// Returns the number of valid moves remaining for this tile
    pub fn remaining_moves(&self) -> usize {
        self.remaining_values
    }

    /// Marks the given value as no longer valid for this tile
    pub fn mark_invalid(&mut self, value: usize) {
        let index = value - 1;
        if self.valid_values[index] {
            self.valid_values[index] = false;
            self.remaining_values -= 1;
        }
    }

    /// Returns the first valid move that is available for this tile or None if there aren't any
    pub fn first_valid_move(&self) -> Option<usize> {
        self.valid_moves().first()
    }

    /// Returns an iterator going through all the valid moves that could be placed at this tile
    pub fn valid_moves<'a>(&self) -> impl Iterator<Item=usize> + 'a {
        self.valid_values.iter().enumerate().filter(|&(_, &v)| v).map(|(i, _)| i + 1)
    }
}

#[derive(Debug, Clone)]
pub struct Sudoku {
    tiles: [[Tile; BOARD_SIZE]; BOARD_SIZE],
    empty_tiles: usize,
}

impl Default for Sudoku {
    fn default() -> Sudoku {
        Sudoku {
            tiles: Default::default(),
            empty_tiles: BOARD_SIZE * BOARD_SIZE,
        }
    }
}

impl fmt::Display for Sudoku {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let thick_vert: &str = "\u{2551}";
        let thin_vert: &str = "\u{2502}";
        let thick_hori: String = repeat("\u{2550}").take(4 * BOARD_SIZE + 1).collect();
        let thin_hori: String = repeat("\u{2500}").take(4 * BOARD_SIZE + 1).collect();

        write!(f, "{}{}", thick_hori, &self.tiles.iter().enumerate().map(|(i, row)| {
            let row = row.iter().enumerate().map(|(j, tile)| {
                let tile_sep = if (j + 1) % BOX_SIZE == 0 {
                    thick_vert
                }
                else {
                    thin_vert
                };

                format!(" {} {}", tile, tile_sep)
            }).collect::<String>();

            let row_sep = if (i + 1) % BOX_SIZE == 0 {
                &thick_hori
            }
            else {
                &thin_hori
            };

            format!("\n{}{}\n{}", thick_vert, row, row_sep)
        }).collect::<String>())
    }
}

impl Sudoku {
    /// Returns true if the sudoku board is completely filled out
    pub fn is_complete(&self) -> bool {
        self.empty_tiles == 0
    }

    /// Places a value on the sudoku board
    pub fn place(&mut self, (row, col): (usize, usize), value: usize) {
        debug_assert!(value > 0 && value <= BOARD_SIZE, "value is not in the right range");
        debug_assert!(row < BOARD_SIZE, "row is not in the right range");
        debug_assert!(col < BOARD_SIZE, "col is not in the right range");
        debug_assert!(self.get((row, col)).value.is_none(), "Placed over already filled tile");

        self.get_mut((row, col)).value = Some(value);
        self.empty_tiles -= 1;

        // The starting row and column of the box that this value belongs in
        let box_row_start = (row / BOX_SIZE) * BOX_SIZE;
        let box_col_start = (col / BOX_SIZE) * BOX_SIZE;

        // This loop updates every number in the same row, column and box as this value to indicate
        // that the value being stored can no longer be used in any of those other items
        for i in 0..BOARD_SIZE {
            // Update items in the same row
            self.get_mut((row, i)).mark_invalid(value);
            // Update items in the same column
            self.get_mut((i, col)).mark_invalid(value);
            // Update items in the same box
            self.get_mut((
                box_row_start + i / BOX_SIZE,
                box_col_start + i % BOX_SIZE
            )).mark_invalid(value);
        }
    }

    pub fn solve(&mut self) {
        if self.is_complete() {
            return;
        }

        while !self.is_complete() {
            let tile = self.tiles.iter().enumerate().filter_map(|(i, row)| {
                row.iter().enumerate().filter(|&(_, tile)| tile.value.is_none())
                    .filter_map(|(j, tile)| {
                        let remaining = tile.remaining_moves();
                        if remaining > 0 {
                            Some((i, j, remaining))
                        }
                        else { None }
                    })
                    .min_by_key(|&(_, _, remaining)| remaining)
            }).min();
            // not solvable because no valid tiles left
            if tile.is_none() {
                break;
            }
            let (i, j, remaining) = tile.unwrap();

            // fill in obvious moves first
            if remaining == 1 {
                let value = {
                    let tile = self.get((i, j));
                    debug_assert!(tile.value.is_none());
                    debug_assert!(tile.remaining_moves() == 1);
                    tile.first_valid_move().unwrap()
                };

                self.place((i, j), value);

                {
                    let tile = self.get((i, j));
                    debug_assert!(tile.value.is_some());
                }
            }
            // make a guess
            else {
                let mut game = (*self).clone();

                {
                    let tile = self.get((i, j));
                    debug_assert!(tile.value.is_none());
                    debug_assert!(tile.remaining_moves() > 0);
                }

                let guess = self.get((i, j)).first_valid_move().unwrap();
                game.place((i, j), guess);
                println!("{}", game);
                println!("Guessing {}, remaining {}", guess, game.empty_tiles);
                game.solve();
                if game.is_complete() {
                    *self = game;
                    return;
                }
            }
        }
    }

    fn get(&self, (row, col): (usize, usize)) -> &Tile {
        &self.tiles[row][col]
    }

    fn get_mut(&mut self, (row, col): (usize, usize)) -> &mut Tile {
        &mut self.tiles[row][col]
    }
}
