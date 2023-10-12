#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "./../include/RC4_PRNG.h"

#define S_BOX_SIZE   256
#define KEY_MAX_SIZE 256

/*
    Boolean data type.
*/
typedef enum {False, True} Bool;

/*
    Swaps single-byte values at the specified addresses.
*/
static void     swap(uint8_t* const x, uint8_t* const y);

/*
    Initializes the S array with the given key.
*/
static uint8_t* initKeySchedulingAlgorithm(const uint8_t* const key, const uint16_t keyLength);

/*
    Generates one byte of the RC4 stream.
    If the resetStream argument is set to True, the current state (current offset) of the stream for this key is reset.
*/
static uint8_t  getPseudoRandomByte(uint8_t* const s, Bool resetStream);

/*
    See RC4_PRNG.h for description.
*/
uint8_t* getPseudoRandomBytesStream(
                                     const uint8_t* const key,
                                     const uint16_t       keyLength,
                                     const uint64_t       streamHeadOffset,
                                     const uint64_t       streamLength
                                   )
{
    uint8_t* streamResult = (uint8_t*) malloc(sizeof(uint8_t) * streamLength);
    if (NULL == streamResult)
    {
        perror("problem with uint8_t* streamResult");    /* if memory allocation failed */
        exit(-1);
    }

    if (keyLength > 256 || 0 == keyLength)
    {
        perror("ERROR: key length must be 0 < keyLength < 256 bytes");
        exit(-1);
    }
    uint8_t* s = initKeySchedulingAlgorithm(key, keyLength);

    if (streamHeadOffset > 0)
        for(uint64_t i = 0; i < streamHeadOffset; i++)
            getPseudoRandomByte(s, False);    /* these values are not needed */

    for(uint64_t i = 0; i < streamLength; i++)
        streamResult[i] = getPseudoRandomByte(s, False);

    getPseudoRandomByte(s, True);    /* resetting current stream offset */

    free(s);
    return streamResult;
}

/*
    See RC4_PRNG.h for description.
*/
void printPseudoRandomBytesStream(
                                     const uint8_t* const stream,
                                     const uint64_t       streamLength
                                 )
{
    printf("value:\n");
	for(int i = 0; i < streamLength; i++){
		printf("%02X ",stream[i]);
		if(i % 16 == 15)
			printf("\n");
	}
}

static void swap(uint8_t* const x, uint8_t* const y)
{
    uint8_t t = *x;
    *x        = *y;
    *y        =  t;
}

static uint8_t* initKeySchedulingAlgorithm(const uint8_t* const key, const uint16_t keyLength)
{
    uint8_t* s = (uint8_t*) malloc(sizeof(uint8_t) * S_BOX_SIZE);
    if (NULL == s)
    {
        perror("problem with uint8_t* s");    /* if memory allocation failed */
        exit(-1);
    }

    for(uint64_t i = 0; i < S_BOX_SIZE; i++)
        s[i] = i;

    for(uint64_t i = 0, j = 0; i < S_BOX_SIZE; i++)
    {
        j = (j + s[i] + key[i % keyLength]) % S_BOX_SIZE;
        swap(s + i, s + j);
    }

    return s;    
}

static uint8_t getPseudoRandomByte(uint8_t* const s, Bool resetStream)
{
    static uint64_t i = 0, j = 0;
    if (True == resetStream)
    {
        i = 0;
        j = 0;
        return 0;
    }

    i = (i + 1)    % S_BOX_SIZE;
    j = (j + s[i]) % S_BOX_SIZE;
    swap(s + i, s + j);

    return s[(s[i] + s[j]) % S_BOX_SIZE];
}