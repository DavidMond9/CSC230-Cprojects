/**
 * @file replace.c
 * @author David Mond (dmmond)
 * Replace file can replace words for the madlib story, using the placeholders to actually replace it in the current line.
 * It also can compute the length and see if it is greater than the max, which would return false, and true if otherwise.
*/

#include "replace.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "input.h"

/**
 * Replaces the placeholder with the word, based off different inputs with the char arrays. This method uses nested for loops to check
 * if it has a match or not with the placeholder, and then branches off into different invalid checks.
 * @param line Char array that represents the line of the story.
 * @param word Char array that represents each word of the line.
 * @param placeholder Char array that represents the placeholder for the story
*/
void replaceWord(char line[LINE_MAX + 1], const char word[FIELD_MAX + 1], const char placeholder[]) 
{
    int lineLength = strlen(line);
    int wordLength = strlen(word);
    int placeholderLength = strlen(placeholder);

    char result[LINE_MAX + 1];
    int resultIndex = 0;
    //If total length is greater than max, exit
    if (lineLength + wordLength - placeholderLength > LINE_MAX) {
        exit(103);
    }

    for (int i = 0; i < lineLength; i++) {
        //Default the match to false
        int match = 1;

        //Check if the placeholder matches starting from the current position
        for (int j = 0; j < placeholderLength; j++) {
            if (line[i + j] != placeholder[j]) {
                match = 0;
                break;
            }
        }
        //If placeholder was found
        if (match) {
            //Check if the line exceeds the maximum length after placeholder replacement
            if (resultIndex + wordLength > LINE_MAX) {
                exit(103);
            }
            //Check if the placeholder is followed by spaces or the end of the line
            int k;
            for (k = i + placeholderLength; k < lineLength; k++) {
                if (line[k] != ' ') {
                    //Exit the loop if something but a space is found
                    break;
                }
            }

            if (k == lineLength || (k < lineLength && line[k] == '\n')) {
                exit(101);
            }

            //Replace the placeholder with the word
            for (int m = 0; m < wordLength; m++) {
                result[resultIndex++] = word[m];
            }

            //Skip the placeholder in the original line
            i += placeholderLength - 1;
        } else {
            //Copy the character from the original line to the result
            result[resultIndex++] = line[i];
        }
    }
    //Null the result string
    result[resultIndex] = '\0';

    //Copy the result back to the original line
    strcpy(line, result);
}

/**
 * Compute the length and see if it is valid compared to the max length of a line.
 * @param line char array to represent the line
 * @param word char array to represent the word
 * @param placeholder char array to represent the placeholder
 * @return returns true if length is valid, false if length is greater than line max.
*/
bool computeLen(char line[LINE_MAX + 1], char word[FIELD_MAX + 1], char placeholder[])
{
    int lengthLine = strlen(line);
    int lengthWord = strlen(word);
    int lengthPlace = strlen(placeholder);

    //Total length
    int res = lengthLine + (lengthWord - lengthPlace);
    //Invalid
    if(res > LINE_MAX){
        return false;
    }
    //Valid
    return true;
}