/**
 * @file sha256.h
 * @author David Mond (dmmond)
 * Header for sha256 file. Defines some magic numbers and has prototypes for all the functions in sha256.c
*/

#ifndef SHA256_H
#define SHA256_H

#include "sha256constants.h"

/** Type used to represent a byte. */
typedef unsigned char byte;

/** Type used to represent a 64-bit value. */
typedef unsigned long word64;

/** Number of bits in a byte. */
#define BBITS 8

/** Size of an input block in bytes. */
#define BLOCK_SIZE 64

/** Size of the hash, in words. */
#define HASH_WORDS 8

/** State of the SHA256 algorithm, including bytes of input data
    waiting to be hashed. */
typedef struct {
  /** Input data not yet hashed. */
  byte pending[ BLOCK_SIZE ];

  /** Number of byes currently in the pending array. */
  int pcount;

  /** Total Length of SHAState*/
  unsigned long long totalLength;

  /** Current hash value. */
  word h[ HASH_WORDS ];
} SHAState;


/** 
 * Performs a circular right rotation on a word value
 * @param val The word to rotate.
 * @param bits The number of bits to rotate by.
 * @return The rotated word.
 */
word rotate(word val, int bits);

/** 
 * The Sigma0 function in the SHA-256 algorithm, applies a series of rotations and shifts
 * @param a The word to be processed.
 * @return The result of Sigma0 function.
 */
word Sigma0(word a);

/** 
 * The Sigma1 function in the SHA-256 algorithm, applies a series of rotations and shifts
 * @param e The word to be processed.
 * @return The result of Sigma1 function.
 */
word Sigma1(word e);

/** 
 * The Ch function in the SHA-256 algorithm, which mixes the bits of words
 * @param e The first word operand.
 * @param f The second word operand.
 * @param g The third word operand.
 * @return The result of the Ch function.
 */
word ChFunction(word e, word f, word g);

/** 
 * The Ma function in the SHA-256 algorithm, which determines the majority bit
 * @param a The first word operand.
 * @param b The second word operand.
 * @param c The third word operand.
 * @return The result of the Ma function.
 */
word MaFunction(word a, word b, word c);

/** 
 * Makes an instance of SHAState for hashing.
 * @return A pointer to the initialized SHAState struct
 */
SHAState *makeState();

/** 
 * Frees everything used by an instance of SHAState
 * @param state The SHAState instance to free
 */
void freeState(SHAState *state);

/** 
 * Extends the 64-byte block into an array of 64 words that will be used in the hash computation
 * @param pending The input 64-byte block to extend
 * @param w The output array of 64 words
 */
void extendMessage(byte const pending[BLOCK_SIZE], word w[BLOCK_SIZE]);

/** 
 * Performs the main compression function of the SHA-256 hash algorithm
 * @param state The current state of the hash computation
 */
void compression(SHAState *state);

/** 
 * Processes input data in blocks, updating the hash computation for each complete block
 * @param state The current state of the hash computation
 * @param data The input data to process
 * @param len The length of the input data
 */
void update(SHAState *state, const byte data[], int len);

/** 
 * Finalizes the hash computation and produces the final hash value
 * @param state The current state of the hash computation
 * @param hash The array where the final hash value will be stored
 */
void digest(SHAState *state, word hash[HASH_WORDS]);

#endif
