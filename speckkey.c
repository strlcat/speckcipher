#include <string.h>
#include "speckdef.h"
#include "specksupport.h"

void makekey(void *out, const char *pwd)
{
	const SPECK_BYTE_TYPE *upwd = pwd;
	SPECK_UNIT_TYPE x[SPECK_NR_BLOCK_UNITS], y[SPECK_NR_BLOCK_UNITS], k[SPECK_NR_BLOCK_UNITS];
	size_t sl = strlen(pwd), i;

	memset(k, 0, SPECK_BLOCK_SIZE);

	if (sl >= SPECK_BLOCK_SIZE) {
		do {
			memcpy(x, upwd, SPECK_BLOCK_SIZE);
			upwd += SPECK_BLOCK_SIZE;
			data_to_words(x, SPECK_BLOCK_SIZE);
			for (i = 0; i < SPECK_NR_BLOCK_UNITS; i++) y[i] = x[i] ^ k[i];
			speck_encrypt_rawblk(y, y, k);
			for (i = 0; i < SPECK_NR_BLOCK_UNITS; i++) k[i] = y[i] ^ x[i];
			data_to_words(y, SPECK_BLOCK_SIZE);
			memcpy(out, y, SPECK_BLOCK_SIZE);
		} while ((sl -= SPECK_BLOCK_SIZE) >= SPECK_BLOCK_SIZE);
	}

	if (sl) {
		memset(x, 0, SPECK_BLOCK_SIZE);
		memcpy(x, out, SPECK_BLOCK_SIZE);
		memset(k, 0, SPECK_BLOCK_SIZE);
		memcpy(k, upwd, sl);
		data_to_words(x, SPECK_BLOCK_SIZE);
		data_to_words(k, SPECK_BLOCK_SIZE);
		speck_encrypt_rawblk(y, x, k);
		for (i = 0; i < SPECK_NR_BLOCK_UNITS; i++) y[i] ^= k[i];
		data_to_words(y, SPECK_BLOCK_SIZE);
		memcpy(out, y, SPECK_BLOCK_SIZE);
	}
	else {
		memset(x, 0, SPECK_BLOCK_SIZE);
		memset(k, 0, SPECK_BLOCK_SIZE);
		speck_encrypt_rawblk(y, x, k);
		data_to_words(y, SPECK_BLOCK_SIZE);
		memcpy(out, y, SPECK_BLOCK_SIZE);
	}

	memset(x, 0, SPECK_BLOCK_SIZE);
	memset(y, 0, SPECK_BLOCK_SIZE);
	memset(k, 0, SPECK_BLOCK_SIZE);
}


int main(int argc, char **argv)
{
	SPECK_BYTE_TYPE key[SPECK_KEY_SIZE];

	if (argc < 2) return 1;

	memset(key, 0, sizeof(key));
	makekey(key, argv[1]);
	mhexdump(key, sizeof(key), 16);
	return 0;
}
