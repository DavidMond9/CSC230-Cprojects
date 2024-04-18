/**
 * @file input.c
 * @author David Mond (dmmond)
 * Reads in input from the given input stream.
*/

#include <stdlib.h>
#include <stdio.h>
#include "input.h"

//Size of buffer
#define SIZE 128

/**
 * This function reads a single line of input from the given input stream and returns it as a string inside a block of dynamically allocated memory.
 * @param fp File to read the input in from.
 * @return returns a character that is a string inside of a block of dynamically allocated memory.
*/
char *readLine(FILE *fp) {
    const int initialSize = SIZE;
    char *buffer = (char *)malloc(initialSize * sizeof(char));
    int position = 0; 
    while (1) {
        //Read a character from the file
        int c = fgetc(fp);
        //Check for end of line or end of file
        if (c == '\n' || c == EOF) {
            //Null-terminate the string
            buffer[position] = '\0';
            //Check for immediate EOF and empty line
            if (c == EOF && position == 0) {  
                free(buffer);
                return NULL;
            } else {
                //Return the line
                return buffer;
            }
        } else {
            //Store the character in the buffer
            buffer[position] = c;
        }
        //Move to the next position in the buffer
        position++;

        //Check if the buffer needs to be enlarged
        if (position >= initialSize) {
            int newSize = initialSize + position;
            char *newBuffer = (char *)realloc(buffer, newSize * sizeof(char));
            //Point to the new buffer
            buffer = newBuffer;
        }
    }
}