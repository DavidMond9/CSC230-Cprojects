/**
 * @file hash.c
 * @author David Mond (dmmond)
 * Main area of the program, gets a SHA256 hash code for a specific message and utilizes different
 functions like update and digest to help create the code.
*/
#include "sha256.h"
#include <stdio.h>
#include <stdlib.h>

#define TWO 2

/**
* Main function, handles the update and digest functions. Checks for error cases with invalid files and usage.
* @return returns an integer for exit success or exit failure.
*/
int main(int argc, char *argv[]) 
{
    // Ensure an argument is provided
    if (argc > TWO) {
        fprintf(stderr, "usage: hash [input_file]\n");
        return EXIT_FAILURE;
    }
    // Initialize SHAState
    SHAState *state = makeState();
    if (!state) {
        return EXIT_FAILURE;
    }

    // Determine input source
    FILE *file = argc == 2 ? fopen(argv[1], "rb") : stdin;
    if (!file) {
        fprintf(stderr, "missing-input-file.txt: No such file or directory\n");
        freeState(state);
        return EXIT_FAILURE;
    }

    // Read and update state with file contents
    byte buffer[BLOCK_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, BLOCK_SIZE, file)) > 0) {
        update(state, buffer, bytesRead);
    }

    if (ferror(file)) {
        freeState(state);
        // Only close the file if it's not stdin
        if (file != stdin) {
           fclose(file);
        }
        return EXIT_FAILURE;
    }

    // Finalize the hashing
    word hash[HASH_WORDS];
    digest(state, hash);

    // Print the hash in hexadecimal
    for (int i = 0; i < HASH_WORDS; i++) {
        printf("%08x", hash[i]);
    }
    printf("\n");

    // Free state at end
    freeState(state);
    if (file != stdin) {
    // Only close the file if it's not stdin
       fclose(file);
    }
    return EXIT_SUCCESS;
}