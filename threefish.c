#include <stdint.h>
#include <stdio.h>
#include "threefish.h"

void threefish(unsigned char *c_n, const unsigned char *p_n, const unsigned char *k_n, const unsigned char *t_n)
{

	uint64_t k[4] = {0x00};
	uint64_t t[2] = {0x00};
	uint64_t c[4] = {0x00};
	uint64_t p[4] = {0x00};
	uint64_t k_gen[5] = {0x00}, keys[19][4] = {0x00}, t_gen[3] = {0x00};
	uint64_t v[73][4] = {0x00}, e[72][4] = {0x00};
	uint64_t f[72][4] = {0x00};
	int nr = 72;

	// Extracting the keys chars to words
	char_to_words(k_n, k, 4);
	// Extracting the t chars to words

	char_to_words(t_n, t, 2);
	// Extracting the plaintext to words

	char_to_words(p_n, p, 4);

	// Building the key generetor for all the next 19 keys
	for (int i = 0; i < 4; i++)
	{
		k_gen[i] = k[i];
		k_gen[4] ^= k[i];
	}
	k_gen[4] ^= 0x1BD11BDAA9FC1A22;
	// Building the t generetor for all the next 19 keys
	for(int i = 0; i < 2; i++) { t_gen[i] = t[i]; }
	t_gen[2] = t_gen[0] ^ t_gen[1];
	// Creating the next 19 keys with the rule on paper
	for (int s = 0; s < 19; s++)
 	{
 		keys[s][0] = k_gen[s%5];
 		keys[s][1] = k_gen[(s+1)%5] + t_gen[s%3];
 		keys[s][2] = k_gen[(s+2)%5] + t_gen[(s+1)%3];
 		keys[s][3] = k_gen[(s+3)%5] + s;
  	}

  	// Giving the value for the starting out
	for (int i = 0; i < 4; i++) { v[0][i] = p[i]; }

	// Inition of ROUNDS
	for (int d = 0; d < nr; d++)
	{
		// Addition of Subkey
		if (d%4 == 0) // In each 4 step there is an adition of the key schedule
		{
			for(int i = 0; i < 4; i++) { e[d][i] = v[d][i] + keys[d/4][i]; }
		}
		else
		{
			for(int i = 0; i < 4; i++) { e[d][i] = v[d][i]; }
		}

		// Mixing
		for (int i = 0; i < 2; i++)
		{
			f[d][2*i] = e[d][2*i] + e[d][2*i + 1];
			f[d][2*i + 1] = ROTA(e[d][2*i + 1], R[i][d%8]); // Rotate the key as defined at the begining
			f[d][2*i + 1] ^= f[d][2*i];
		}

		// Updating values for the next ROUND with Permutation
		for (int i = 0; i < 4; i++) { v[d + 1][i] = f[d][PR[i]]; }
	}

	// Ciphertext
	// We sum the last key scheduled with the last round
 	for(int i = 0; i < 4; i++) { c[i] = v[nr][i] + keys[nr/4][i]; }

 	// Building the ciphertext from words to character vector
 	words_to_char(c, c_n, 4);

}
