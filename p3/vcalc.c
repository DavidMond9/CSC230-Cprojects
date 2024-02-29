/**
 * @file vcalc.c
 * @author David Mond (dmmond)
 * Main class of the program, handles parsing operands, parsing expressions, parsing
 * statements, and has the main() function of the whole project. Uses
 * standard file commands to read in the input and produce the output
 * and uses a global long array to store variables for expressions.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "vigesimal.h"
#include "check.h"
#include <ctype.h>

/** Input-file argument */
#define INPUT_ARG 1  

/** Output-file argument */
#define OUTPUT_ARG 2

/** Length of variables array */
#define LENGTH 26

/**
 * Global long array to store variables 'a' to 'z'. Default all the values
 * to 0.
*/
long variables[LENGTH] = {0}; 

/**
 * Used for informing the user about command line, used in the main() method, prints out to stderr.
*/
static void usage()
{
  fprintf( stderr, "usage: vcalc INPUT-FILE [OUTPUT-FILE]\n" );
  exit( EXIT_FAILURE );
}  

/**
 * Parses an operand, checks to see if the character is the start of an assignment
 * for a variable or a mathematical expression. If assignment, use global array,
 * if math, use parse number.
 * @param val pointer to long to store the value of the number/variable read in.
 * @param vname pointer to name of the variable if eligible, if not, set to '\0'
 * @param input pointer to input file to parse the operand from.
*/
bool parseOperand(long *val, char *vname, FILE *input)
{
    //Read in next character
    int ch = skipWhitespace(input);
    //If end of file, invalid
    if(ch == EOF){
      return false;
    }
    //Default variable name to null, change later if it is a varaiable
    *vname = '\0';
    //If base 20 number (math expression)
    if((ch >= 'A' && ch <= 'T') || ch == '-'){
        //Put character back so it can be parsed
      ungetc(ch, input);    
      //Parse number
      return parseNumber(val, input);
    }
    //Else if variable 
    else if(ch >= 'a' && ch <= 'z'){
      //Assign variable name
      *vname = (char) ch;
      //Get value from global array
      *val = variables[ch - 'a'];
      return true;
    }
    //Not base 20 number or variable, invalid
    else {
      return false;
    }
}

/**
 * Parses the expression which can start with an operand or a number depending on 
 * if it is an assignment or a mathmetical expression. 3 parameters, one that 
 * starts the expression, one that stores the results, and one that is the input
 * file.
 * @param result pointer to a long that stores the result of the expression.
 * @param left start of the parsing, the beginning of the expression.
 * @param input pointer to a file which is the input file we will read in from
*/
bool parseExpression(long *result, long left, FILE *input) 
{
    // Store first operand.
    long currentOperand = left;  
    // New long to help with result
    long val = 0;
    // Variable name for assignments.
    char vname;
    // Variable to track if operator has been read.
    char op = 0;
    // Sets result to the first operand
    *result = currentOperand;

    //Run till we break out
    while (true) { 
        //If an operator has been read
        if (op != 0) { 
            //Parse operand
            if (!parseOperand(&val, &vname, input)) {
                return false; 
            }
            //If variable name was read
            if(vname != '\0') {
                val = variables[vname - 'a'];
            }
            //Switch case for the math, with the operator
            switch (op) {
                //Add
                case '+': if(!add(result, *result, val)){
                    return false;
                }; break;
                //Subtract
                case '-': if(!subtract(result, *result, val)){
                    return false;
                }; break;
                //Multiply
                case '*': if(!multiply(result, *result, val)){
                    return false;
                }; break;
                //Divide
                case '/': 
                    if (val == 0) return false; 
                    if(!divide(result, *result, val)){
                        return false;
                    }; break; 
                    break;
                //None, invalid
                default: return false; 
            }
        } 
        //Get next char
        int ch = skipWhitespace(input); 
        //If semicolon, expression is done.
        if (ch == ';') {
            break; 
        }
        //If an operator has been read and we have another letter, invalid
        if(ch >= 'A' && ch <= 'Z' && op != 0){
            return false;
        }
        //If the next char is not an operator, put it back and check if we read an operator
        if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
            ungetc(ch, input);
            //If an operator has not been read, assignment statement, parse operand
            if(op == 0){
                if (!parseOperand(&val, &vname, input)) {
                    return false; 
                }
                //Set result to the value of parse operand
                *result = val;
                //Continue through the loop now that the first number was read
                continue; 
            }
        }
        //Store operator for the switch case
        op = (char)ch; 
    }  
    //Success
    return true; 
}

/**
 * Parses the statement which can start with an operand or a number depending on 
 * if it is an assignment or a mathmetical expression. 3 parameters, one that 
 * counts the statement number, one that is the input file, and one that is the output
 * file.
*/
bool parseStatement(int stmtNum, FILE *input, FILE *output) 
{
    // Value to store the result.
    long val;
    // Initliaze variable name to null, can change later.
    char vname = '\0';
    // No variable read in yet, set to false.
    bool isVariable = false;

    //Attempt to parse the first operand, which could be a number or a variable name
    if (!parseOperand(&val, &vname, input)) {
        //If invalid operand and end of file not reached.
        if (!feof(input)) {
            fprintf(output, "S%d: invalid\n", stmtNum);
        }
        //Skip to next line
        int ch;
        while((((ch = fgetc(input)) != EOF) && ch != ';')){
            
        }
        return false;
    }

    //Check if a variable name was parsed instead of a number.
    if (vname != '\0') {
        isVariable = true;
    }

    //Get the next char
    int nextChar = skipWhitespace(input);

    //If the next character is '=', assignment statement.
    if (nextChar == '=') {
        if (isVariable) {
            //Parse the expression after first number.
            if (!parseExpression(&val, 0, input)) {
                fprintf(output, "S%d: invalid\n", stmtNum);
                //Skip to next line
                int ch;
                while((((ch = fgetc(input)) != EOF) && ch != ';')){
                     
                }
                return false;
            }
            //Set variable value to val that was read from expression
            variables[vname - 'a'] = val;
        } else {
            //If not a variable, invalid assignment.
            fprintf(output, "S%d: invalid\n", stmtNum);
            //Skip to next line
            int ch;
            while((((ch = fgetc(input)) != EOF) && ch != ';')){
            
            }
            return false;
        }
    } else {
        //If next character not '=', expression.
        //Put back whitespace char
        ungetc(nextChar, input);
        //Stores the value of result
        long result;
        
        //Parse the expression starting with val as the initial value.
        if (!parseExpression(&result, val, input)) {
            fprintf(output, "S%d: invalid\n", stmtNum);
            //Skip to next line
            int ch;
            while((((ch = fgetc(input)) != EOF) && ch != ';')){
            
            }
            
            return false;
        }
        //Valid output, print out number 
        fprintf(output, "S%d: ", stmtNum);
        printNumber(result, output);
        fprintf(output, "\n");
        
    }
    
   //Continue to next statement
    return true;
}

/**
 * Main function of the project. Uses an input and output file with different
 * amounts of arguments. Calls parseStatement to run through the input files and 
 * parse all the numbers as a base 20 calculator. Checks for invalid file cases
 * such as no input file or too many arguments.
 * @param argc number of arguments must be greater than input arguments
 * @param argv char array to hold the different arguments
 * @return returns EXIT_SUCCESS, an int for success
*/
int main(int argc, char *argv[]) 
{
    //Check if the correct number of arguments has been provided.
    if (argc < INPUT_ARG) {
        //Call usage to let user know what to do
        usage(); 
        exit(EXIT_FAILURE);
    }

    //Attempt to open the input file
    FILE *input = fopen(argv[INPUT_ARG], "r");
    if (!input) {
        //Print to stderr
        fprintf(stderr, "Can't open file: %s\n", argv[INPUT_ARG]);
        exit(EXIT_FAILURE);
    }

    //Determine the output destination based on the output file argument
    //Use ternary operator for quicker logic with files
    FILE *output = (argc > OUTPUT_ARG) ? fopen(argv[OUTPUT_ARG], "w") : stdout;
    if (argc > OUTPUT_ARG && !output) {
        //Case 20, print to stderr using argv
        fprintf(stderr, "Can't open file: %s\n", argv[OUTPUT_ARG]);
        //Close input file
        fclose(input); 
        exit(EXIT_FAILURE);
    }

    int stmtNum = 1;
    //Process each statement until an invalid one is encountered and EOF is reached.
    while (parseStatement(stmtNum++, input, output) || !feof(input)) {  
        
    }

    //Close the input file
    fclose(input);
    //Close the output file if it's not stdout
    if (output != stdout) {
        fclose(output);
    }
    //Success
    return EXIT_SUCCESS; 
}