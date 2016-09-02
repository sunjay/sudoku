#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "inputhandler.h"

#define CHUNK_SIZE 64

char* getline(FILE* fp) {
    // read until the end of line or end of file
    char c;
    char* line = malloc(sizeof(char)*CHUNK_SIZE);

    int i = 0;
    int size = CHUNK_SIZE;
    while (true) {
        c = getc(fp);
        if (c == EOF) {
            return NULL;
        }

        if (i >= size) {
            size += CHUNK_SIZE;
            line = realloc(line, sizeof(char)*size);
        }

        line[i++] = c;
        if (c == EOL) {
            break;
        }
    }

    return realloc(line, sizeof(char)*i);
}
