/**
 * @file madlib.c
 * @author David Mond (dmmond)
 * Main portion of the program, reads the words, checks for invalid input,
 * and actually creates the madlib story with the nouns, adjectives, verbs, and adverbs.
*/


#include "input.h"
#include "replace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Main function for the program. 
 * @return returns exit success if program runs successfully
*/
int main() 
{
    char noun1[FIELD_MAX + 1];
    char noun2[FIELD_MAX + 1];
    char verb[FIELD_MAX + 1];
    char adjective[FIELD_MAX + 1];
    char adverb[FIELD_MAX + 1];
    //Read in the noun1
    readWord(noun1);
    //Read in the noun2
    readWord(noun2);
    //Read in the verb
    readWord(verb);
    //Read in the adjective
    readWord(adjective);
    //Read in the adverb
    readWord(adverb);
    int lengthNoun1 = strlen(noun1);
    int lengthNoun2 = strlen(noun2);
    int lengthVerb = strlen(verb);
    int lengthAdjective = strlen(adjective);
    int lengthAdverb = strlen(adverb);
    //If any of the words were not read in, exit101
    if(lengthNoun1 == 0 || lengthNoun2 == 0 || lengthVerb == 0 || lengthAdjective == 0 || lengthAdverb == 0){
        exit(101);
    }

    char line[LINE_MAX + 1];
    //While the line can be read in  
    while (readLine(line)) {
        replaceWord(line, noun1, "<noun1>");
        replaceWord(line, noun2, "<noun2>");
        replaceWord(line, verb, "<verb>");
        replaceWord(line, adjective, "<adjective>");
        replaceWord(line, adverb, "<adverb>");
        // Check if the line exceeds 100 characters
        if (strlen(line) > 100) {
            exit(103);
        }
        //If there is a leftover placeholder after reading in all words, it is an invalid placeholder, exit104
        for(int i = 0; i < strlen(line); i++){
            if(line[i] == '<'){
                exit(104);
            }
        }
        //Print output
        printf("%s\n", line);
    }
    //Success
    return EXIT_SUCCESS;
}