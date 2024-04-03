/**
 * @file sha256.c
 * @author David Mond (dmmond)
 * SHA256 file. Handles the SHA-256 algorithm, uses different functions to compress, rotate, and update bits and words. 
 * Uses an SHAState as well for hashing, and makes sure all memory is freed.
*/

#include "sha256.h"
#include "sha256constants.h"
#include <stdlib.h>
#include <string.h>

#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define TWENTYFOUR 24
#define SIXTEEN 16
#define THIRTYTWO 32
#define FIFTYSIX 56
#define SIXTYFOUR 64


/** 
 * Performs a circular right rotation on a word value
 * @param val The word to rotate.
 * @param bits The number of bits to rotate by.
 * @return The rotated word.
 */
word rotate(word val, int bits)
{
   return (val >> bits) | (val << (THIRTYTWO - bits));
}

/** 
 * The Sigma0 function in the SHA-256 algorithm, applies a series of rotations and shifts
 * @param a The word to be processed.
 * @return The result of Sigma0 function.
 */
word Sigma0(word a)
{
   return (rotate(a, 2)) ^ (rotate(a, 13)) ^ (rotate(a, 22));
}

/** 
 * The Sigma1 function in the SHA-256 algorithm, applies a series of rotations and shifts
 * @param e The word to be processed.
 * @return The result of Sigma1 function.
 */
word Sigma1(word e)
{
   return (rotate(e, 6)) ^ (rotate(e, 11)) ^ (rotate(e, 25));
}

/** 
 * The Ch function in the SHA-256 algorithm, which mixes the bits of words
 * @param e The first word operand.
 * @param f The second word operand.
 * @param g The third word operand.
 * @return The result of the Ch function.
 */
word ChFunction(word e, word f, word g)
{
   return (e & f) ^ (~e & g);
}

/** 
 * The Ma function in the SHA-256 algorithm, which determines the majority bit
 * @param a The first word operand.
 * @param b The second word operand.
 * @param c The third word operand.
 * @return The result of the Ma function.
 */
word MaFunction(word a, word b, word c)
{
   return (a & b) ^ (a & c) ^ (b & c);
}

/** 
 * Extends the 64-byte block into an array of 64 words that will be used in the hash computation
 * @param pending The input 64-byte block to extend
 * @param w The output array of 64 words
 */
void extendMessage(byte const pending[], word w[]) 
{
    for (int i = 0; i < SIXTEEN; i++) {
        w[i] = (word)pending[FOUR * i] << TWENTYFOUR |
               (word)pending[FOUR * i + 1] << SIXTEEN |
               (word)pending[FOUR * i + TWO] << EIGHT |
               (word)pending[FOUR * i + THREE];
    }

    for (int i = SIXTEEN; i < SIXTYFOUR; i++) {
        word s0 = rotate(w[i - 15], SEVEN) ^ rotate(w[i - 15], 18) ^ (w[i - 15] >> THREE);
        word s1 = rotate(w[i - TWO], 17) ^ rotate(w[i - TWO], 19) ^ (w[i - TWO] >> 10);
        w[i] = w[i - SIXTEEN] + s0 + w[i - SEVEN] + s1;
    }
}

/** 
 * Performs the main compression function of the SHA-256 hash algorithm
 * @param state The current state of the hash computation
 */
void compression(SHAState *state) 
{
    word a, b, c, d, e, f, g, h, T1, T2, W[SIXTYFOUR];
    extendMessage(state->pending, W);

    a = state->h[0];
    b = state->h[1];
    c = state->h[TWO];
    d = state->h[THREE];
    e = state->h[FOUR];
    f = state->h[FIVE];
    g = state->h[SIX];
    h = state->h[SEVEN];

    for (int i = 0; i < SIXTYFOUR; i++) {
        T1 = h + Sigma1(e) + ChFunction(e, f, g) + constant_k[i] + W[i];
        T2 = Sigma0(a) + MaFunction(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }

    state->h[0] += a;
    state->h[1] += b;
    state->h[TWO] += c;
    state->h[THREE] += d;
    state->h[FOUR] += e;
    state->h[FIVE] += f;
    state->h[SIX] += g;
    state->h[SEVEN] += h;
}

/** 
 * Makes an instance of SHAState for hashing.
 * @return A pointer to the initialized SHAState struct
 */
SHAState *makeState() 
{
    SHAState *state = (SHAState *)malloc(sizeof(SHAState));
    if (!state) {
        freeState(state);
        return NULL;
    }

    memcpy(state->h, initial_h, HASH_WORDS * sizeof(word));
    state->pcount = 0;
    state->totalLength = 0; 

    return state;
}

/** 
 * Frees everything used by an instance of SHAState
 * @param state The SHAState instance to free
 */
void freeState(SHAState *state)
{
    free(state);
}

/** 
 * Processes input data in blocks, updating the hash computation for each complete block
 * @param state The current state of the hash computation
 * @param data The input data to process
 * @param len The length of the input data
 */
void update(SHAState *state, const byte data[], int len) 
{
    size_t index = 0;

    // Update total length for every byte added
    state->totalLength += len;

    // Fill the remainder of the pending buffer
    if (state->pcount) {
        size_t fill = BLOCK_SIZE - state->pcount;
        if (len >= fill) {
            memcpy(state->pending + state->pcount, data, fill);
            compression(state);
            index += fill;
            state->pcount = 0;
        } else {
            memcpy(state->pending + state->pcount, data, len);
            state->pcount += len;
            return;
        }
    }

    // Process message to 64-byte chunks
    while (index + BLOCK_SIZE <= len) {
        memcpy(state->pending, data + index, BLOCK_SIZE);
        compression(state);
        index += BLOCK_SIZE;
    }

    // Save remaining data in state
    if (index < len) {
        memcpy(state->pending, data + index, len - index);
        state->pcount = len - index;
    }
}

/** 
 * Finalizes the hash computation and produces the final hash value
 * @param state The current state of the hash computation
 * @param hash The array where the final hash value will be stored
 */
void digest(SHAState *state, word hash[HASH_WORDS]) 
{
    // Add the padding 1 bit followed by 0 bits
    state->pending[state->pcount++] = 0x80;

    // If there is not enough space for the padding and length bytes, pad and compress
    if (state->pcount > BLOCK_SIZE - EIGHT) {
        while (state->pcount < BLOCK_SIZE) {
            state->pending[state->pcount++] = 0x00;
        }
        compression(state);
        state->pcount = 0;
    }

    // Pad with zeros up to the last 8 bytes
    while (state->pcount < BLOCK_SIZE - EIGHT) {
        state->pending[state->pcount++] = 0x00;
    }

    // Append the length in bits as a 64-bit big-endian integer
    unsigned long long bits = state->totalLength << THREE;
    for (int i = 0; i < EIGHT; i++) {
        state->pending[BLOCK_SIZE - EIGHT + i] = (byte)(bits >> (FIFTYSIX - (i * EIGHT)));
    }

    compression(state);

    // Output the final hash value
    for (int i = 0; i < HASH_WORDS; ++i) {
        hash[i] = state->h[i];
    }
}