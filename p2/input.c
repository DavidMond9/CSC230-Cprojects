/**
 * @file input.c
 * @author David Mond (dmmond)
 * Gets the input for reading words, reading lines, and making sure all input is valid. It checks for line length, invalid characters,
 * and more.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "replace.h"
#include "input.h"

/**
 * Checks to see if the character is an allowed character.
 * @param c character to be checked.
 * @return returns true if it is a valid character, false if not
*/
bool isAllowedCharacter(int c) 
{
    //ASCII checks
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '\\' || c == '-';
}

/**
 * Reads the word into a new char array, and checks to see if the word is valid in length and characters.
 * @param word char array that the word is being read into.
*/
void readWord(char word[FIELD_MAX + 1]) 
{
    //Current Character
    int c;
    //Index of word char array
    int idx = 0;
    //For lines with extra spaces
    bool hasNonSpace = false;

    // Read the word
    while ((c = getchar()) != '\n' && c != EOF) {
        if (isAllowedCharacter(c)) {
            if (idx < FIELD_MAX) {
                word[idx++] = c;
                //For cases with an extra line of spaces
                hasNonSpace = true;
            } else {
                //Word length exceeds the maximum allowed length
                exit(102); 
            }
            //Exit if a character that isn't a space is encountered after reading the word
        } else if (hasNonSpace) {
            exit(102); 
        }
    }

    //Null the word
    word[idx] = '\0';
}

/**
 * Reads the line into a new char array which is the parameter.
 * @param line the char array that is being read into.
 * @return returns true if it was successfully read into, false if not.
*/
bool readLine(char line[LINE_MAX + 1]) 
{
    //Index of line char array
    int idx = 0;
    //Current character
    int c = getchar();
    //Case where the end of file is reached, and no characters are read
    if (c == EOF) {
        return false;
    }

    while (c != '\n' && c != EOF) {
        //Case where the line exceeds the maximum length
        if (idx >= LINE_MAX) {
            exit(103);
        }

        line[idx] = c;
        idx++;

        c = getchar();
    }

    //Check for the end of file
    if (c == EOF && idx == 0) {
        //Case where the end of file is reached, and no characters are read
        return false;
    }
    //Null the string
    line[idx] = '\0';
    return true;
}