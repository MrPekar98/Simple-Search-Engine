#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Simple hashing function.
unsigned int32_checksum(const char *str)
{
	unsigned res = 0;
	unsigned length = strlen(str), i;

	for (i = 0; i < length; i++)
	{
		res += (unsigned) str[i];
	}

	return res;
}

// Fletcher's checksum.
uint16_t int16_checksum(const void *data, unsigned length)
{
    uint16_t sum1 = 0, sum2 = 0;
    unsigned i;

    for (i = 0; i < length; i++)
    {
        sum1 = (sum1 + *((uint8_t *) (data + i))) % 255;
        sum2 = (sum2 + sum1) % 255;
    }

    return (sum2 << 8) | sum1;
}

#ifdef __cplusplus
}
#endif

#endif