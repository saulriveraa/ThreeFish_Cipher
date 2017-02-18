#include <stdint.h>

// Rule for the conversion of 8 constrains to 1 word
static uint64_t r[8] = {0x1, 0x100, 0x10000, 0x1000000, 0x100000000,
					0x10000000000, 0x1000000000000, 0x100000000000000};

// Mixing Rule constants for the mixing
static int const R[2][8] = {{14, 52, 23, 5, 25, 46, 58, 32},
						{16, 57, 40, 37, 33, 12, 22, 32}};

// Rule Permutation
static int const PR[4] = {0, 3, 2, 1};

static inline uint64_t ROTA(uint64_t x, int n)
{
    return (x << n) | (x >> (64 - n));
};

static inline void char_to_words(const unsigned char *k_n, uint64_t *k, int n)
{
    int aux_11 = 0;

    for (int i = 0; i < n; i++)
	{
		for( int o = 0; o < 8; o++)
		{
			k[i] ^= k_n[aux_11 + o] * r[o];
		}
		aux_11 += 8;
	}

};

static inline void words_to_char(uint64_t *c, unsigned char *c_n, int n)
{
    int aux_11 = 0;

    for (int i = 0; i < n; i++)
	{
		for( int o = 0; o < 8; o++)
		{ // We rotate the word to get at the end the last byte and then extract it
			c_n[aux_11 + o] = (c[i] >> 8*o) & 0xff;
		}
		aux_11 += 8;
	}
};


/* Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */


/* Under key at k and tweak at t, encrypt 32 bytes of plaintext at p and store it at c. */
void threefish(unsigned char *c, const unsigned char *p, const unsigned char *k, const unsigned char *t);
