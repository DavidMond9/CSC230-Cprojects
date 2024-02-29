/**
 * @file check.c
 * @author David Mond (dmmond)
 * Checks for arithmetic overflow for addition, subtraction, multiplication, and 
 * division. 
*/

#include "check.h"
#include <limits.h>
/**
 * Addition check for overflow.
 * @param result Pointer to get the value of the addition.
 * @param a Value 1 being added.
 * @param b Value 2 being added.
 * @return returns true if successfully added, false if not
*/
bool add(long *result, long a, long b)
{
   if (((b > 0) &&  (a > 0) && (a + b < 0)) || 
        (((b < 0) && (a < 0) && (a + b > 0) ))) { 
        return false;
    }
   *result = a + b;
   return true;
}

/**
 * Subtraction check for overflow.
 * @param result Pointer to get the value of the subtraction.
 * @param a Value 1 being subracted.
 * @param b Value 2 being subtracted.
 * @return returns true if successfully subtracted, false if not
*/
bool subtract(long *result, long a, long b)
{
    if (((b < 0) && (a > LONG_MIN + b)) || 
        ((b > 0) && (a < LONG_MIN + b))) { 
        return false;
    }
    *result = a - b;
    return true;
}
/**
 * Multiplication check for overflow.
 * @param result Pointer to get the value of the multiplication.
 * @param a Value 1 being multiplied.
 * @param b Value 2 being multiplied.
 * @return returns true if successfully multiplied, false if not
*/
bool multiply(long *result, long a, long b)
{
    if (a == 0 || b == 0) {
        *result = 0;
        return true;
    }

    if ((a > 0 && b > 0 && (a > LONG_MAX / b)) || 
        (a < 0 && b < 0 && (b < LONG_MAX / a)) || 
        (a < 0 && b > 0 && (a < LONG_MIN / b)) || 
        (a > 0 && b < 0 && (b < LONG_MIN / a))) {
        return false;
    }
    *result = a * b;
    return true;
}

/**
 * Division check for overflow.
 * @param result Pointer to get the value of the division.
 * @param a Value 1 being divided.
 * @param b Value 2 being divided.
 * @return returns true if successfully divided, false if not
*/
bool divide(long *result, long a, long b)
{
    if((a == LONG_MIN && b == -1) || (b == 0)){
        return false;
    }
    *result = a / b;
    return true;
}
