/**
 * @file vigesimal.c
 * @author David Mond (dmmond)
 * Parse's numbers and converts them to base-20 values. Reads in up to the LONG_MIN
 * and the LONG_MAX values. Prints numbers to the output in base-20.
*/

#include "vigesimal.h"
#include "check.h"
#include <ctype.h>
#include <limits.h> 
#include <stdlib.h>

/** Base of the number system we're implementing. */
#define BASE 20

/** Maximum number of characters needed to print the largest number. */
#define MAX_NUMBER_LEN 16


/**
 * Reads the next character and skips the whitespace.
 * @param input Pointer to an input file to read in from.
 * @return returns an int that represents the next character.
*/
int skipWhitespace(FILE *input)
{
    //Get the character until no more white space or end of file
    int ch;
    while((((ch = fgetc(input)) != EOF) && isspace(ch))){
    }
    return ch;
}

/**
 * Parses the next number and converts it from Base 20 to Base 10.
 * @param val Pointer to long which will store the result of the number.
 * @param input Pointer to the input file which we will parse from.
*/
bool parseNumber(long *val, FILE *input) 
{
    //Next Character
    int ch = skipWhitespace(input);
    bool isNegative = false;
    //If leading with negative, make result negative.
    if (ch == '-') {
        isNegative = true;
        //Skip the '-'
        ch = fgetc(input);
    }
    //Result of number
    long result = 0;
    //Valid number?
    bool isValid = false;
    while (ch >= 'A' && ch <= 'T') {
        //Calculate the digit
        int digit = ch - 'A';
        result = result * BASE + digit;
        //Number was read
        isValid = true;
        ch = fgetc(input); 
    }
    
    //If the next character isn't the end or space, put it back
    if (!isspace(ch) && ch != EOF) {
        ungetc(ch, input);
    }
    //Change to negative if '-' was detected
    if(isNegative){
        *val = -result;
    }
    else {
        *val = result;
    }
    return isValid;
}

/**
 * Prints a number and converts it from Base 10 to Base 20.
 * @param val Long value that keeps track of the conversion
 * @param output Pointer to the output file which we will print to.
*/
void printNumber(long val, FILE *output) 
{
    //If negative, print a '-'
    if (val < 0) {
        fputc('-', output);
    }
    //List to store number
    char list[MAX_NUMBER_LEN + 1];
    //Idx of list
    int idx = 0;
    do { 
        //Conversion
        long digit = val % BASE;
        val /= BASE;
        if(digit >= 0){
            list[idx++] = 'A' + digit;
        }
        else{
            list[idx++] = 'A' + -digit;
        }
    } while (val != 0); //Keep going till conversion is complete
    list[idx] = '\0';

    //Print out list that stores base 20 number
    for(int i = idx - 1; i >= 0; i--){
        fputc(list[i], output);
    }
}   

