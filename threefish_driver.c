#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "threefish.h"

#define CLOCK(x) (x >> 8 | (x ^ (x >> 7) ^ (x >> 6) ^ (x >> 2)) << 24)
void rnd(unsigned char *b, int len) {
	static uint32_t s = 0xFEEDFACE;
	for(int i=0; i<len; i++, s=CLOCK(s)) b[i] = (unsigned char)s;
}

int main() {
	

	unsigned char c[32], k[32], t[16];

	rnd(k,32); rnd(t,16);

	char p[32] = {"hola"};

	threefish(c, p, k, t);

	printf("The chiper message is: ");
	for(int i = 0; i < 32; i++)
		printf("%c", c[i]);

	printf("\n");
		
	return 0;
}
