/**
 * @file driver.c
 * @author David Mond (dmmond)
 * Main part of the program, reads from standard in and makes a map that can use many commands.
 * These commands can set variables, get variables, remove, or add. driver.c handles all this logic and returns with exit success or failure.
*/

#include "map.h"
#include "value.h"
#include "input.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/** Length of buffer. */
#define BUFFER 1024
/** Number for sscanf matches. */
#define ARG 2

/** Main method that handles all commands for the map. Returns an integer for exit success or failure.
* @param argc number of arguments that is being passed
* @param argv array of arguments
* @return returns an int for exit success or failure.
*/
int main(int argc, char *argv[]) 
{
    FILE *fp = stdin;
    //Create the map
    Map *m = makeMap();
    char *line;
    printf("cmd> ");
    //Keep processing commands while you can read a line
    while ((line = readLine(fp)) != NULL) {
        printf("%s\n", line);

        char command[BUFFER];
        char key[BUFFER];
        char valueStr[BUFFER];
        //Commands
        if (sscanf(line, "%99s", command) == 1) {
            //Exit program
            if (strcmp(command, "quit") == 0) {
                free(line);
                break;
            //Get size
            } 
            else if (strcmp(command, "size") == 0) {
                printf("%d\n", mapSize(m));
            //Set variable
            } 
            else if (strcmp(command, "set") == 0) {
                if (sscanf(line, "%*s %1024s %1023s", key, valueStr) == ARG){
                    bool valid = true;
                    for (int i = 0; key[i]; i++) {
                        if (!isprint((unsigned char)key[i])) {
                            valid = false;
                            printf("invalid\n");
                            continue;
                        }
                    }
                    Value *val = NULL;
                    if(valid) {
                        //Either int or double
                        if (isdigit(valueStr[0]) || valueStr[0] == '-' || valueStr[0] == '+') {
                            val = parseInteger(valueStr) ?: parseDouble(valueStr);
                            //If starts with a quote, its a string
                        } 
                        else if (valueStr[0] == '\"') {
                            val = parseString(valueStr);
                        }
                        if (val) {
                            mapSet(m, key, val);
                        }
                        else { 
                            printf("invalid\n");
                        }
                    }
                }
            } else if (strcmp(command, "get") == 0) {
                // Skip past get
                char *keyStart = line + strlen("get ");
                
                // Skip any initial whitespace
                while (*keyStart && isspace((unsigned char)*keyStart)) {
                    keyStart++;
                }

                // Invalid if its just get
                if (*keyStart == '\0') {
                    printf("invalid\n");
                } else {
                    // Find the end of the key which is the end of the line.
                    char *keyEnd = keyStart;
                    while (*keyEnd && *keyEnd != '\n' && *keyEnd != '\r') {
                        keyEnd++;
                    }

                    // Compute the key length
                    size_t keyLen = keyEnd - keyStart;

                    if (keyLen < sizeof(key) - 1) {
                        // Copy the key and null terminate it
                        memcpy(key, keyStart, keyLen);
                        key[keyLen] = '\0';

                        Value *val = mapGet(m, key);
                        if (val) {
                            // Convert the value to a string and print it
                            char *valStr = val->toString(val);
                            printf("%s\n", valStr);
                            free(valStr);
                        } else {
                            printf("invalid\n");
                        }
                    }
                }
                //Handle remove command
            } else if (strcmp(command, "remove") == 0) {
                if (sscanf(line, "%*s %1023s", key) == 1) {
                    mapRemove(m, key);
                }
                // Handle Plus command
            } else if (strcmp(command, "plus") == 0) {
                if (sscanf(line, "%*s %1023s %1023s", key, valueStr) == ARG) {
                    Value *val1 = mapGet(m, key);
                    Value *addVal = NULL;
                    if (isdigit(valueStr[0]) || valueStr[0] == '-' || valueStr[0] == '+' || valueStr[0] == ' ') {
                        addVal = parseInteger(valueStr) ? : parseDouble(valueStr);
                    } else if (valueStr[0] == '\"') {
                        addVal = parseString(valueStr);
                    }

                    // Check if both values are not NULL and are the same type
                    if (val1 && addVal && val1->toString == addVal->toString) {
                        val1->plus(val1, addVal);
                    } else {
                        printf("invalid\n");
                    }

                    //Free the temporary value created for addition
                    if (addVal) {
                        destroyValue(addVal);
                    }
                }
            }
        }
        printf("\n");
        printf("cmd> ");
        free(line);
    }

    freeMap(m);
    return EXIT_SUCCESS;
}