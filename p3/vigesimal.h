/**
 * @file vigesimal.h
 * @author David Mond (dmmond)
 * Header class for vigesimal. Writes prototypes for parseNumber, skipWhiteSpace,
 * and printNumber.
*/

// The prototypes need the bool and FILE types.
#include <stdio.h>
#include <stdbool.h>
#include "check.h"  
/**
 * Reads the next character and skips the whitespace.
 * @param input Pointer to an input file to read in from.
 * @return returns an int that represents the next character.
*/
int skipWhitespace(FILE *input);
/**
 * Parses the next number and converts it from Base 20 to Base 10.
 * @param val Pointer to long which will store the result of the number.
 * @param input Pointer to the input file which we will parse from.
*/
bool parseNumber(long *val, FILE *input);
/**
 * Prints a number and converts it from Base 10 to Base 20.
 * @param val Long value that keeps track of the conversion
 * @param output Pointer to the output file which we will print to.
*/
void printNumber(long val, FILE *output);

