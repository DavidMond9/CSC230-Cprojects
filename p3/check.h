/**
 * @file check.h
 * @author David Mond (dmmond)
 * Checks for arithmetic overflow for addition, subtraction, multiplication, and 
 * division. 
*/

// The function prototypes need the bool type.
#include <stdbool.h>
/**
 * Addition check for overflow.
 * @param result Pointer to get the value of the addition.
 * @param a Value 1 being added.
 * @param b Value 2 being added.
 * @return returns true if successfully added, false if not  
*/
bool add(long *result, long a, long b);
/**
 * Subtraction check for overflow.
 * @param result Pointer to get the value of the subtraction.
 * @param a Value 1 being subracted.
 * @param b Value 2 being subtracted.
 * @return returns true if successfully subtracted, false if not
*/
bool subtract(long *result, long a, long b);
/**
 * Multiplication check for overflow.
 * @param result Pointer to get the value of the multiplication.
 * @param a Value 1 being multiplied.
 * @param b Value 2 being multiplied.
 * @return returns true if successfully multiplied, false if not
*/
bool multiply(long *result, long a, long b);
/**
 * Division check for overflow.
 * @param result Pointer to get the value of the division.
 * @param a Value 1 being divided.
 * @param b Value 2 being divided.
 * @return returns true if successfully divided, false if not
*/
bool divide(long *result, long a, long b);


