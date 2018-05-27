#include <stdio.h>

#include "inputhandler.h"

#define EOL '\n'

/**
 * Attempts to read exactly the given target number of characters on a line
 * in the given file pointer.
 * A newline character is expected exactly after `target` characters has
 * been read. It is an error if that is not found or if it is found before.
 *
 * The read characters are placed in the given buffer
 * Returns 0 if the line was read successfully, -1 otherwise
 * Guaranteed to only fill the buffer up to target length
 * If the function returns -1, there is no guarantee about the buffer's 
 * contents and whether they will be mutated or not
 */
int getnline(FILE* fp, int target, char buffer[]) {
    char c;
    for (int i = 0; i < target; i++) {
        c = getc(fp);
        if (c == EOF || c == EOL) {
            return -1;
        }

        buffer[i] = c;
    }

    // Must end with EOL or EOF
    c = getc(fp);
    if (c != EOF && c != EOL) {
        return -1;
    }

    return 0;
}

