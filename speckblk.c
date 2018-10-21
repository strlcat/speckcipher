#include <string.h>
#include "speckdef.h"

void speck_encrypt_block(const void *key, void *out, const void *in)
{
	SPECK_UNIT_TYPE x[SPECK_NR_BLOCK_UNITS], k[SPECK_NR_KEY_UNITS];
	SPECK_UNIT_TYPE *uout = out;

	memcpy(x, in, SPECK_BLOCK_SIZE);
	data_to_words(x, SPECK_BLOCK_SIZE);
	memcpy(k, key, SPECK_KEY_SIZE);
	data_to_words(k, SPECK_BLOCK_SIZE);
	speck_encrypt_rawblk(uout, x, k);
	memset(k, 0, SPECK_KEY_SIZE);
	data_to_words(uout, SPECK_BLOCK_SIZE);
}
