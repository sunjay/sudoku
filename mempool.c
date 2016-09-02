/**
 * A simple memory pool implementation designed to avoid excessive
 * calls to malloc (which is quite slow)
 *
 * Author: Sunjay Varma (www.sunjay.ca)
 */

#include <stdlib.h>
#include <stdbool.h>

#include "sudoku.h"

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

/**
 * Allocates a single frame memory pool for use
 * Returns NULL if something goes wrong
 */
MemoryPool* newMemoryPool(void) {
    MemoryPool* pool = malloc(sizeof(MemoryPool));
    if (!pool) {
        return NULL;
    }

    pool->next = NULL;
    pool->searchStart = 0;

    for (int i = 0; i < POOL_SIZE; i++) {
        pool->occupied[i] = false;
    }

    return pool;
}

/**
 * Appends an additional frame to the given memory pool
 * 
 * Returns 0 if the operation is successful and -1 otherwise
 */
int extendMemoryPool(MemoryPool* pool) {
    if (pool->next != NULL) {
        // tried to overwrite an existing memory pool (memory leak)
        return -1;
    }

    pool->next = newMemoryPool();
    if (pool->next == NULL) {
        return -1;
    }

    return 0;
}

/**
 * Frees ALL frames starting at the given memory pool frame
 */
void freeMemoryPool(MemoryPool* pool) {
    MemoryPool* current = pool;
    while (current != NULL) {
        MemoryPool* tmp = current->next;
        free(current);
        current = tmp;
    }
}

/**
 * Returns the address of the next free SudokuBoard slot
 * Allocates a new frame if necessary
 * Returns NULL if something goes wrong
 */
SudokuBoard* allocSudoku(MemoryPool* pool) {
    // Technically, this if statement isn't needed because the loop will
    // not run if this condition isn't true. It does however help make
    // the code more understandable, so I'm leaving it in
    if (pool->searchStart < POOL_SIZE) {
        int start = pool->searchStart;

        for (int i = start; i < POOL_SIZE; i++) {
            if (!pool->occupied[i]) {
                pool->occupied[i] = true;
                pool->searchStart = i + 1;
                return &pool->slots[start];
            }
        }
    }

    // No space left in this frame, look in the next one

    // Create the next frame if there isn't any yet
    if (pool->next == NULL) {
        if (extendMemoryPool(pool) == -1) {
            return NULL;
        }
    }

    return allocSudoku(pool->next);
}

/**
 * Marks the given board as reusable for the next call to alloc
 * This does not automatically free memory pool frames
 * Frames must be freed using freeMemoryPool()
 */
void freeSudoku(MemoryPool* pool, SudokuBoard* board) {
    for (int i = 0; i < POOL_SIZE; i++) {
        if ((&pool->slots[i]) == board) {
            pool->occupied[i] = false;

            // Only want to set searchStart if starting the search at this
            // index will consume the current searchStart index as well
            // This way, we're always filling the pool from the lowest possible index
            if (i < pool->searchStart) {
                pool->searchStart = i;
            }
            return;
        }
    }

    // continue searching the next frame if there is any

    if (pool->next != NULL) {
        return freeSudoku(pool->next, board);
    }
}

