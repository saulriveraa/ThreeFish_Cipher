#include <stdint.h>

void bytes_to_words(const unsigned char *a, uint64_t *b, int len)
{
	int max = len/8, aux_1 = 0;

	// RULE FOR EXTRATION OF INFO TO WORDS
	uint64_t r[8] = {0x1, 0x100, 0x10000, 0x1000000, 0x100000000,
					0x10000000000, 0x1000000000000, 0x100000000000000};

	for (int i = 0; i < max; i++)
	{
		for( int o = 0; o < 8; o++)
		{
			b[i] ^= a[aux_1 + o] * r[o];
		}
		aux_1 += 8;
	}
}

uint64_t ROT(uint64_t x, int n)
{
	return (uint64_t) (x << n) | (x >> (64 - n));
} 

void words_to_bytes(uint64_t *a, unsigned char *b, int len)
{
	int aux_1 = 0;

 	for (int i = 0; i < 4; i++)
	{
		for( int o = 0; o < 8; o++)
		{
			b[o + aux_1] = (a[i] >> 8*o) & 0xff;
			printf("%llx\n", (a[i] >> 8*o) & 0xff);
		}
		aux_1 += 8;
	}
}

/* Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */
/* Under key at k and tweak at t, encrypt 32 bytes of plaintext at p and store it at c. */
void threefish(unsigned char *c, const unsigned char *p, const unsigned char *k, const unsigned char *t);
