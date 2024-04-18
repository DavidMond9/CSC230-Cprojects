/**
 * @file value.c
 * @author David Mond (dmmond)
 * Holds an integer value, double value, and a string value. Can parse doubles, integers, and strings, along with destroyed and adding them.
*/

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/** Type used to represent a subclass of Value that holds an integer. */
typedef struct {
  // Superclass fields.
  char *(*toString)( Value const *v );
  bool (*plus)( Value *v, Value const *x );
  void (*destroy)( Value *v );

  // Subclass fields.
  int val;
} IntegerValue;

/** Maximum length of a 32-bit integer as a string. */
#define INTEGER_LENGTH 11

/** Buffer length.*/
#define BUFFER 2

/** Stirng to String addition. */
#define STR 3

/**
 * Converts integer value to a dynamically allocated string representation.
 *
 * @param v Pointer to the Value structure.
 * @return Dynamically allocated string representing the integer.
 */
static char *integerToString( Value const *v )
{
  // Get v as a pointer to the subclass struct.
  IntegerValue *this = (IntegerValue *) v;

  // Convert to a dynamically allocated string.
  char *str = (char *) malloc( INTEGER_LENGTH + 1 );
  sprintf( str, "%d", this->val );
  return str;
}

/**
 * Adds an integer value to the current integer value in place.
 *
 * @param v Pointer to the current Value structure.
 * @param x Pointer to the Value to add.
 * @return True if operation is successful, false otherwise.
 */
static bool integerPlus( Value *v, Value const *x )
{
  // We can use a function pointer to make sure x is also an
  // integrer.
  if ( x->toString != integerToString )
    return false;
  
  // Get the parameters as IntegerValue poitners.
  IntegerValue *this = (IntegerValue *) v;
  IntegerValue *that = (IntegerValue *) x;

  // Add the value in x to v.
  this->val += that->val;
  return true;
}

/**
 * Frees the memory allocated for integer Value structure.
 *
 * @param v Pointer to the Value structure to destroy.
 */
static void integerDestroy( Value *v )
{
  // All the memory for an integer is in one big block.
  free( v );
}

/**
 * Parses an integer from a given string.
 *
 * @param str The string to parse.
 * @return Pointer to a newly allocated Value structure or NULL on failure.
 */
Value *parseInteger( char const *str )
{
  // Try to parse an integer from str.  The buffer is to make sure
  // there's no extra, non-space characters after the integer value.
  int ival;
  char buffer[ BUFFER ];

  if ( sscanf( str, "%d%1s", &ival, buffer ) != 1 )
    return NULL;

  // Make a new instance of an integer value.
  IntegerValue *v = (IntegerValue *) malloc( sizeof( IntegerValue ) );
  v->toString = integerToString;
  v->plus = integerPlus;
  v->destroy = integerDestroy;
  v->val = ival;

  // Return as a pointer to the superclass.
  return (Value *)v;
}

/** This is the maximum number of characters I could get from a double value,
    printed with %f. */
#define DOUBLE_LENGTH 317

typedef struct {
  char *(*toString)(Value const *v);
  bool (*plus)(Value *v, Value const *x);
  void (*destroy)(Value *v);

  double val; 
} DoubleValue;

/**
 * Declaration for doubleToString.
 *
 * @param v Pointer to the Value structure.
 * @return Dynamically allocated string representing the double.
 */
static char *doubleToString(Value const *v);

/**
 * Declaration for doublePlus.
 *
 * @param v Pointer to the current Value structure.
 * @param x Pointer to the Value to add.
 * @return True if operation is successful, false otherwise.
 */
static bool doublePlus(Value *v, Value const *x);

/**
 * Declaration for doubleDestroy
 *
 * @param v Pointer to the Value structure to destroy.
 */
static void doubleDestroy(Value *v);

/**
 * Converts double value to a dynamically allocated string representation.
 *
 * @param v Pointer to the Value structure.
 * @return Dynamically allocated string representing the double.
 */
static char *doubleToString(Value const *v) 
{
  DoubleValue *dv = (DoubleValue *)v;
  char *str = (char *)malloc(DOUBLE_LENGTH + 1);
  if (str) {
    sprintf(str, "%.6f", dv->val); 
  }
  return str;
}

/**
 * Adds a double value to the current double value in place.
 *
 * @param v Pointer to the current Value structure.
 * @param x Pointer to the Value to add.
 * @return True if operation is successful, false otherwise.
 */
static bool doublePlus(Value *v, Value const *x) 
{
  DoubleValue *dv1 = (DoubleValue *)v;
  Value *xv = (Value *)x;
  if (xv->toString != doubleToString) {
    return false; 
  }
  DoubleValue *dv2 = (DoubleValue *)x;
  dv1->val += dv2->val;
  return true;
}

/**
 * Frees the memory allocated for double Value structure.
 *
 * @param v Pointer to the Value structure to destroy.
 */
static void doubleDestroy(Value *v) 
{
  free(v);
}

/**
 * Parses a double from a given string, skipping leading and trailing whitespaces.
 *
 * @param str The string to parse.
 * @return Pointer to a newly allocated Value structure or NULL on failure.
 */
Value *parseDouble(char const *str) 
{
  // Skip leading whitespace
  while (isspace((unsigned char)*str)) {
    str++;
  }

  char *endptr;
  double dval = strtod(str, &endptr);

  // Skip trailing whitespace
  while (isspace((unsigned char)*endptr)) {
    endptr++;
  }

  if (*endptr != '\0') {
    return NULL; 
  }

  // Allocate a new DoubleValue
  DoubleValue *dv = (DoubleValue *)malloc(sizeof(DoubleValue));
  if (!dv) return NULL;

  // Set up the methods
  dv->toString = doubleToString;
  dv->plus = doublePlus;
  dv->destroy = doubleDestroy;
  dv->val = dval; 

  return (Value *)dv;
}


// Structure to represent a string value
typedef struct {
  char *(*toString)(Value const *v);
  bool (*plus)(Value *v, Value const *x);
  void (*destroy)(Value *v);

  char *val;
} StringValue;

/**
 * Declaration for stringToString.
 *
 * @param v Pointer to the Value
 * @return Dynamically allocated string representation with quote
 */
static char *stringToString(Value const *v);
/**
 * Declaration for stringPlus.
 *
 * @param v Pointer to the current Value 
 * @param x Pointer to the Value to concatenate
 * @return True if successful, false otherwise.
 */
static bool stringPlus(Value *v, Value const *x);
/**
 * Declaration for stringDestroy.
 *
 * @param v Pointer to the Value to destroy.
 */
static void stringDestroy(Value *v);

/**
 * Converts string value to a dynamically allocated string representation with quotes
 *
 * @param v Pointer to the Value
 * @return Dynamically allocated string representation with quote
 */
static char *stringToString(Value const *v) 
{
  StringValue *sv = (StringValue *)v;
  char *str = (char *)malloc(strlen(sv->val) + STR); 
  if (str) {
    sprintf(str, "\"%s\"", sv->val); 
  }
  return str;
}

/**
 * Concatenates string values
 *
 * @param v Pointer to the current Value 
 * @param x Pointer to the Value to concatenate
 * @return True if successful, false otherwise.
 */
static bool stringPlus(Value *v, Value const *x) 
{
  StringValue *sv1 = (StringValue *)v;
  StringValue *sv2 = (StringValue *)x;
  char *newVal = realloc(sv1->val, strlen(sv1->val) + strlen(sv2->val) + 1);
  //combine strings and vals
  strcat(newVal, sv2->val); 
  sv1->val = newVal;
  return true;
}

/**
 * Frees the memory allocated for string Value
 *
 * @param v Pointer to the Value to destroy.
 */
static void stringDestroy(Value *v) 
{
  StringValue *sv = (StringValue *)v;
  free(sv->val);
  free(sv);
}

/**
 * Parses a string value from a given string between quotes.
 *
 * @param str The string to parse.
 * @return Pointer to a new Value or NULL if fails
 */
Value *parseString(char const *str) 
{
  // Find the first quote
  const char *firstQuote = strchr(str, '\"');

  // Find the second quote
  const char *secondQuote = strchr(firstQuote + 1, '\"');
  if (!secondQuote) {
    return NULL; 
  }

  // Make sure everything after the second quote is whitespace until the end of the string
  for (const char *p = secondQuote + 1; *p; p++) {
    if (!isspace((unsigned char)*p)) {
      return NULL; 
    }
  }

  // memory for the string value
  size_t len = secondQuote - firstQuote - 1;
  char *val = (char *)malloc(len + 1);  

  // Copy the string between the quotes
  strncpy(val, firstQuote + 1, len);
  val[len] = '\0'; 

  StringValue *sv = (StringValue *)malloc(sizeof(StringValue));

  // Set up the methods
  sv->toString = stringToString;
  sv->plus = stringPlus;
  sv->destroy = stringDestroy;
  sv->val = val;

  return (Value *)sv;
}

/**
 * Destroys a given Value structure by freeing its memory.
 *
 * @param v Pointer to the Value to destroy.
 */
void destroyValue(Value *v) 
{
  if (v) {
    v->destroy(v);
  }
}