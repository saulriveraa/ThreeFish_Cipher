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

int main() 
{
	unsigned char p_n[32] = {
		0xce, 0xfa, 0xed, 0xfe, 0x63, 0xe8, 0x50, 0x49, 
		0x0a, 0x30, 0xb3, 0xe6, 0xa8, 0x56, 0x48, 0x2d,
		0xf5, 0xf2, 0xf5, 0x58, 0x66, 0xb2, 0x1a, 0x9b,
		0x52, 0x41, 0x86, 0xd2, 0x81, 0xc4, 0xc8, 0xa2};

	unsigned char k_n[32] = {
		0x3a, 0x47, 0xb4, 0x95, 0x66, 0xef, 0xa4, 0x67,
		0xdc, 0x0e, 0x1c, 0xb7, 0x4d, 0x45, 0x69, 0x77,
		0x81, 0x63, 0x80, 0x2d, 0x28, 0x7a, 0x0d, 0xd6,
		0xbe, 0x0b, 0x7a, 0xe5, 0x68, 0x95, 0x7b, 0xae};

	unsigned char t_n[16] = {
		0x4d, 0x69, 0x00, 0x68, 0x69, 0x72, 0x70, 0x45,
		0xde, 0x4f, 0xb3, 0x11, 0x89, 0x37, 0xba, 0x63};

	unsigned char c_n[32] = {0x00};

	// Mixing Rule
	int const R[2][8] = {{14, 52, 23, 5, 25, 46, 58, 32}, 
						{16, 57, 40, 37, 33, 12, 22, 32}};

	// Rule Permutation
	int const PR[4] = {0, 3, 2, 1};

	uint64_t k[4] = {0x00};
	uint64_t t[2] = {0x00};
	uint64_t c[4] = {0x00};
	uint64_t p[4] = {0x00};
	uint64_t k_gen[5] = {0x00}, keys[19][4] = {0x00}, t_gen[3] = {0x00};
	uint64_t v[73][4] = {0x00}, e[72][4] = {0x00};
	uint64_t f[72][4] = {0x00};
	int nr = 72, ns = 19;

	bytes_to_words(k_n, k, 32);
	bytes_to_words(t_n, t, 16);
	bytes_to_words(p_n, p, 32);

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
			f[d][2*i + 1] = ROT(e[d][2*i + 1], R[i][d%8]) ^ f[d][2*i];
		}
		
		// Updating values for the next ROUND with Permutation
		for (int i = 0; i < 4; i++) { v[d + 1][i] = f[d][PR[i]]; }

	}

	// Ciphertext

 	for(int i = 0; i < 4; i++) { c[i] = v[nr][i] ^ keys[nr/4][i]; }

 	for(int i = 0; i < 4; i++) {printf("%llx ", c[i]);}

 	words_to_bytes(c, c_n, 32);


}