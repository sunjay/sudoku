#ifndef __MEMPOOL_H
#define __MEMPOOL_H

#include <stdbool.h>

#include "sudoku.h"

// The number boards allocated in each memory pool frame
// The actual size in bytes is POOL_SIZE*sizeof(SudokuBoard)
#define POOL_SIZE 1024

/**
 * Represents a single frame in the memory pool
 */
typedef struct MemoryPool {
    SudokuBoard slots[POOL_SIZE];
    bool occupied[POOL_SIZE];
    // A hint used to start the search for a new memory pool
    // Always set to the lowest free space in the pool
    int searchStart;
    struct MemoryPool* next;
} MemoryPool;

// Memory pool management functions
MemoryPool* newMemoryPool(void);
int extendMemoryPool(MemoryPool*);
void freeMemoryPool(MemoryPool*);

// Memory pool sudoku board allocation functions
SudokuBoard* allocSudokuBoard(MemoryPool*);
void deallocSudokuBoard(MemoryPool*, SudokuBoard*);

#endif
