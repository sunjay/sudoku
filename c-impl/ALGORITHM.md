Sudoku Solving Algorithm
========================

Summary
-------
This algorithm works by first attempting to find all spaces
where there is only one possible value for that space. Those
values are filled automatically. After that step, the solver
goes through each tile, makes a guess and then attempts to find
a solution from there. Each time a value is placed, the number
of possible values for each other square goes down. When those
tiles reach only a single possibility, the first step of the
algorithm goes in and fills them automatically.

There are a few more things to this algorithm as well, those details
are described below.

Is This A Good/Fast Algorithm?
------------------------------
I don't claim that this is the fastest or best algorithm for 
solving sudoku puzzles, however I know that it is VERY good.

Of the 21,885 puzzles tested, 87.2% were completed in less
than 1 second. 81.9% were completed in less than half of a
second. 71.1% were completed in less than 0.1 seconds.

Overall Approach
----------------

1. Attempt to fill in as many "obvious" solutions as possible.
    Obvious solutions are tiles that only have one
    possible value.
2. If the board is now complete (all tiles are filled), return
    the solved board.
3. For the remaining tiles, find the tile with the smallest number
    of remaining possible solutions.
4. Make a guess -- Place a valid possible value on one of the tiles.
    Repeat step 1 until the board is complete.

Storing The Possible Values
---------------------------
In order to make the algorithm more efficient, it's important to
keep an in-memory cache of the values that are currently valid
for each tile. Any tile that hasn't been filled yet has an array of
possible values that can be placed at that position. When placing
a tile on the board, all related caches need to be updated.

Just updating related tiles turns out to be much more efficient than
the alternative which is re-generating all of the possible
solutions for each tile on every iteration.

Finding The Best Tile To Guess On
---------------------------------
Not all tiles are the same. This algorithm works recursively until
a solution is found or not. Every guess made eliminates a possible
value from the corresponding row, column and box. 

You can think of each guess as forming a "branch" which, if 
followed, will lead you to either a complete solution or no solution
at all.

Since this algorithm works by going through each branch until it
finds the one branch that does lead to the final solution, it's
important to pick the tile which will result in the shortest search.

The "best" tile to traverse is the tile with the smallest number of
possible solutions. If a tile only has 2 possible values that can
be placed on it, there are only 2 branches to follow all the way
to the end.

Questions?
----------
I haven't covered every last detail on this page. If you really
want me to talk about something else, or if something wasn't very
clear, please don't hesitate to contact me.

Send me questions about this using http://www.sunjay.ca/contact
I'll get back to you with an answer.

You can also learn a lot by reading the code. :)