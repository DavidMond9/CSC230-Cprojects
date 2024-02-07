/**
 * @file replace.h
 * @author David Mond (dmmond)
 * Header for the replace, creates the header/paths for the functions in replace.c
*/

#include <stdbool.h>

/** Maximum length for replacement string. */
#define FIELD_MAX 24

/** Maximum length for line of text. */
#define LINE_MAX 100

/**
 * Replaces the placeholder with the word, based off different inputs with the char arrays. This method uses nested for loops to check
 * if it has a match or not with the placeholder, and then branches off into different invalid checks.
 * @param line Char array that represents the line of the story.
 * @param word Char array that represents each word of the line.
 * @param placeholder Char array that represents the placeholder for the story.
*/
void replaceWord(char line[LINE_MAX + 1], const char word[FIELD_MAX + 1], const char placeholder[]);

/**
 * Compute the length and see if it is valid compared to the max length of a line.
 * @param line char array to represent the line
 * @param word char array to represent the word
 * @param placeholder char array to represent the placeholder
 * @return returns true if length is valid, false if length is greater than line max.
*/
bool computeLen(char line[LINE_MAX + 1], char word[FIELD_MAX + 1], char placeholder[]);

