#include <string.h>
#include "speckdef.h"
#include "specke.h"

void specke_init_iv(struct specke_stream *specke, const void *key, const void *iv)
{
	memset(specke, 0, sizeof(struct specke_stream));
	memcpy(specke->key, key, SPECK_KEY_SIZE);
	if (iv) memcpy(specke->iv, iv, SPECK_BLOCK_SIZE);
	specke->tidx = 0;
}

void specke_init(struct specke_stream *specke, const void *key)
{
	specke_init_iv(specke, key, NULL);
}

void specke_emit(void *dst, size_t szdst, struct specke_stream *specke)
{
	SPECK_BYTE_TYPE *udst = dst;
	size_t sz = szdst, trem;

	if (!dst && szdst == 0) {
		memset(specke, 0, sizeof(struct specke_stream));
		return;
	}

	if (specke->tidx > 0) {
		trem = SPECK_BLOCK_SIZE-specke->tidx;

		if (szdst <= trem) {
			memcpy(udst, &specke->tmp[specke->tidx], szdst);
			specke->tidx += szdst;
			if (specke->tidx >= SPECK_BLOCK_SIZE) specke->tidx = 0;
			return;
		}

		memcpy(udst, &specke->tmp[specke->tidx], trem);
		udst += trem;
		sz -= trem;
		specke->tidx = 0;
	}

	if (sz >= SPECK_BLOCK_SIZE) {
		do {
			speck_encrypt_rawblk(specke->iv, specke->iv, specke->key);
			memcpy(udst, specke->iv, SPECK_BLOCK_SIZE);
			data_to_words(udst, SPECK_BLOCK_SIZE);
			udst += SPECK_BLOCK_SIZE;
		} while ((sz -= SPECK_BLOCK_SIZE) >= SPECK_BLOCK_SIZE);
	}

	if (sz) {
		speck_encrypt_rawblk(specke->iv, specke->iv, specke->key);
		memcpy(specke->tmp, specke->iv, SPECK_BLOCK_SIZE);
		data_to_words(specke->tmp, SPECK_BLOCK_SIZE);
		memcpy(udst, specke->tmp, sz);
		specke->tidx = sz;
	}
}
