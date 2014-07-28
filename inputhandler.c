#include <stdio.h>

#define BUFFER_SIZE 512

static char* buffer = NULL;

/**
getline() reads an entire line from stream, storing the address of
the buffer containing the text into *lineptr.  The buffer is null-
terminated and includes the newline character, if one was found.

If *lineptr is set to NULL and *n is set 0 before the call, then
getline() will allocate a buffer for storing the line.  This buffer
should be freed by the user program even if getline() failed.

Alternatively, before calling getline(), *lineptr can contain a
pointer to a malloc(3)-allocated buffer *n bytes in size.  If the
buffer is not large enough to hold the line, getline() resizes it
with realloc(3), updating *lineptr and *n as necessary.

In either case, on a successful call, *lineptr and *n will be updated
to reflect the buffer address and allocated size respectively.
*/
int getline(char** lineptr, size_t* n, FILE* stream) {
	if (n < 0) {
		printf("Invalid buffer size n given.\n");
		exit(0);
	}

	int buffer_length = n;
	int i = 0;
	while (true) {
		if (buffer_length <= n) {
			buffer_length += BUFFER_SIZE;
			*lineptr = (char*)realloc(*lineptr, sizeof(char)*buffer_length);
		}

	}

	if (n == 0) {
		*lineptr = NULL;
	}
}