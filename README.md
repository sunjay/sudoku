# sudoku

This implementation of a sudoku solver is written in the [Rust programming
language]. The solver was initially programmed in C and the [source code for the
C implementation][c-impl] is still available. The C code was last edited in
2016.

When I initially ported it to Rust, I intentionally tried to make the Rust code
look as much like C as possible. This was so that I could get it correct and
then work on making it idiomatic Rust. I finished doing that pretty quickly and
decided to do a cursory benchmark just to see if there was any difference. I was
expecting the Rust code to be slower and first and that I would need to optimize
it quite a bit. Surprisingly, it turned out to be the complete opposite. The
Rust implementation was **orders of magnitude** faster than my C implementation.
I'm not the best C programmer in the world, so I can't say my C code was
completely optimal. The amazing part here is that my Rust code mostly resembled
the C code but was still **significantly faster**.

Some non-scientific timing data I collected at the time:

* C implementation: 9.356 seconds
* Rust implementation: 0.644 seconds

This is the time it took to solve 95 "hard" puzzles (`samples/hard95.txt`). The
difference between the two implementations is huge and I imagine that as I
optimize the Rust implementation and make it more idiomatic we will see even
more of a speedup.

## Algorithm

The basic idea of this algorithm is very similar to how we solve sudoku puzzles
by hand. The `Sudoku` data structure is setup to do the same book keeping that
you might do on a piece of paper. It tracks which numbers could possibly be
placed on any given tile and continuously attempts to guess on the tiles that it
can't narrow down completely.

The C implementation needed to put in a lot of effort to minimize the guessing
in order to be fast. The Rust implementation turned out to be faster with just a
simple guess and check strategy.

More optimization is still possible and will likely be attempted in the future.

## Building & Running

If you haven't installed Rust and Cargo already, you can do so at [rustup.rs]

**You must build the Rust code with --release in order for it to solve sudoku
puzzles fast.**

```bash
$ cargo run --release < samples/hard95.txt
```

The solver expects the input in the following format:

* 9 lines terminated by a trailing newline
* 9 numbers in each line
* 0 represents an empty space

The solver will read as many boards in this format as it can, output their
solutions one at a time and then exit.

Example board:

```
400000805
030000000
000700000
020000060
000080400
000010000
000603070
500200000
104000000
```

If you just want to build and not run, you can use the following command:

```bash
$ cargo build --release
```

This will produce an executable that you can run with the following command:

```bash
$ ./target/release/solve
```

[Rust programming language]: https://www.rust-lang.org
[c-impl]: https://github.com/sunjay/sudoku/tree/master/c-impl
[rustup.rs]: https://rustup.rs/
