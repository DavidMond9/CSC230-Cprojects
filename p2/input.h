/**
 * @file input.h
 * @author David Mond (dmmond)
 * Header for the input, creates the header/paths for the functions in input.c
*/

#include "replace.h"
#include <stdbool.h>

/**
 * Reads the word into a new char array, and checks to see if the word is valid in length and characters.
 * @param word char array that the word is being read into.
*/
void readWord(char word[FIELD_MAX + 1]);

/**
 * Reads the line into a new char array which is the parameter.
 * @param line the char array that is being read into.
 * @return returns true if it was successfully read into, false if not.
*/
bool readLine(char line[LINE_MAX + 1]);

/**
 * Checks to see if the character is an allowed character.
 * @param c character to be checked.
 * @return returns true if it is a valid character, false if not.
*/
bool isAllowedCharacter(int c);
