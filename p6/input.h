/**
 * @file input.h
 * @author David Mond (dmmond)
 * Declaration of the function for reading input lines.
 */


#include <stdio.h>

/**
 * This function reads a single line of input from the given input stream and returns it as a string inside a block of dynamically allocated memory.
 * @param fp File to read the input in from.
 * @return returns a character that is a string inside of a block of dynamically allocated memory.
*/
char *readLine(FILE *fp);