#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "RC4_PRNG.h"

int main(void)
{
	uint8_t  key[]        = { 0x01, 0x02, 0x03, 0x04, 0x05 };
	uint16_t keyLength    = sizeof(key);
	uint64_t streamOffset = 0;
	uint64_t streamLength = 32;

	uint8_t* result = getPseudoRandomBytesStream(key, keyLength, streamOffset, streamLength);
	printPseudoRandomBytesStream(result, streamLength);
	free(result);
	return 0;
}
