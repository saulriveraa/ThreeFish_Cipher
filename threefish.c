#include <stdint.h>
#include <stdio.h>
#include "threefish.h"

#if 0 /* TODO change to 0 after you complete this task */
/*

TODO FREE TEXT ASSIGNMENT

If there was a FAQ for this assignment, what question and answer would've helped you the most?

Q: <FILL ME IN>

A: <FILL ME IN>

*/
#warning "FAIL you have not completed the free text assignment"
#endif

#define ROT(x, n) (uint64_t) (x << n) | (x >> (64 - n))

void threefish(unsigned char *c_n, const unsigned char *p_n, const unsigned char *k_n, const unsigned char *t_n) 
{
	// Mixing Rule
	int const R[2][8] = {{14, 52, 23, 5, 25, 46, 58, 32}, 
						{16, 57, 40, 37, 33, 12, 22, 32}};

	// Rule Permutation
	int const PR[4] = {0, 3, 2, 1};

	// RULE FOR EXTRATION OF INFO TO WORDS
	uint64_t r[8] = {0x1, 0x100, 0x10000, 0x1000000, 0x100000000,
					0x10000000000, 0x1000000000000, 0x100000000000000};

	uint64_t k[4] = {0x00};
	uint64_t t[2] = {0x00};
	uint64_t c[4] = {0x00};
	uint64_t p[4] = {0x00};
	uint64_t k_gen[5] = {0x00}, keys[19][4] = {0x00}, t_gen[3] = {0x00};
	uint64_t v[73][4] = {0x00}, e[72][4] = {0x00};
	uint64_t f[72][4] = {0x00};
	int nr = 72, ns = 19;
	int aux_11 = 0;

	for (int i = 0; i < 4; i++)
	{
		for( int o = 0; o < 8; o++)
		{
			k[i] ^= k_n[aux_11 + o] * r[o];
		}
		aux_11 += 8;
	}

	aux_11 = 0;
	for (int i = 0; i < 2; i++)
	{
		for( int o = 0; o < 8; o++)
		{
			t[i] ^= t_n[aux_11 + o] * r[o];
		}
		aux_11 += 8;
	}

	aux_11 = 0;
	for (int i = 0; i < 4; i++)
	{
		for( int o = 0; o < 8; o++)
		{
			p[i] ^= p_n[aux_11 + o] * r[o];
		}
		aux_11 += 8;
	}


	for (int i = 0; i < 4; i++)
	{
		k_gen[i] = k[i];
		k_gen[4] ^= k[i];
	}
	k_gen[4] ^= 0x1BD11BDAA9FC1A22;

	for(int i = 0; i < 2; i++) { t_gen[i] = t[i]; }
	t_gen[2] = t_gen[0] ^ t_gen[1];
	
	for (int s = 0; s < 19; s++)
 	{
 		keys[s][0] = k_gen[s%5]; 
 		keys[s][1] = k_gen[(s+1)%5] + t_gen[s%3];
 		keys[s][2] = k_gen[(s+2)%5] + t_gen[(s+1)%3];
 		keys[s][3] = k_gen[(s+3)%5] + s;
  	}
  	printf("==== KEYS ====\n");
  	for(int o = 0; o < ns; o++){
  		printf("%02d: ", o);
 	for (int i = 0; i < 4; i++) {printf("%016llx ", keys[o][i]); }
 		printf("\n");
 	}

  	// Giving the value for the starting out
	for (int i = 0; i < 4; i++) { v[0][i] = p[i]; }

	// Inition of ROUNDS
	for (int d = 0; d < nr; d++)
	{
		// Addition of Subkey
		if (d%4 == 0) 
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
			f[d][2*i + 1] = ROT(e[d][2*i + 1], R[i][d%8]);
			f[d][2*i + 1] ^= f[d][2*i];
		}
		
		// Updating values for the next ROUND with Permutation
		for (int i = 0; i < 4; i++) { v[d + 1][i] = f[d][PR[i]]; }
	}
	
	printf("==== RNDS ====\n");
  	for(int o = 0; o < nr + 1; o++){
  		printf("%02d: ", o);
 	for (int i = 0; i < 4; i++) {printf("%016llx ", v[o][i]); }
 		printf("\n");
 	}

	// Ciphertext

 	for(int i = 0; i < 4; i++) { c[i] = v[nr][i] + keys[nr/4][i]; }

 	for(int i = 0; i < 4; i++) { printf("%llx ", c[i]);; }

 	aux_11= 0;

 	for (int i = 0; i < 4; i++)
	{
		for( int o = 0; o < 8; o++)
		{
			c_n[aux_11 + o] = (c[i] >> 8*o) & 0xff;
		}
		aux_11 += 8;
	}

}