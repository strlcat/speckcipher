#include <string.h>
#include "speckdef.h"
#include "specke.h"

void specke_init_iv(struct specke_stream *specke, const void *key, const void *iv)
{
	memset(specke, 0, sizeof(struct specke_stream));
	memcpy(specke->key, key, SPECK_KEY_SIZE);
	data_to_words(specke->key, SPECK_KEY_SIZE);
	if (iv) {
		memcpy(specke->iv, iv, SPECK_BLOCK_SIZE);
		data_to_words(specke->iv, SPECK_BLOCK_SIZE);
	}
	specke->carry_bytes = 0;
}

void specke_init(struct specke_stream *specke, const void *key)
{
	specke_init_iv(specke, key, NULL);
}

void specke_emit(void *dst, size_t szdst, struct specke_stream *specke)
{
	SPECK_BYTE_TYPE *udst = dst;
	size_t sz = szdst;

	if (!dst && szdst == 0) {
		memset(specke, 0, sizeof(struct specke_stream));
		return;
	}

	if (specke->carry_bytes > 0) {
		if (specke->carry_bytes > szdst) {
			memcpy(udst, specke->carry_block, szdst);
			memmove(specke->carry_block, specke->carry_block+szdst, specke->carry_bytes-szdst);
			specke->carry_bytes -= szdst;
			return;
		}

		memcpy(udst, specke->carry_block, specke->carry_bytes);
		udst += specke->carry_bytes;
		sz -= specke->carry_bytes;
		specke->carry_bytes = 0;
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
		memcpy(udst, specke->iv, sz);
		data_to_words(udst, SPECK_BLOCK_SIZE);
		udst = (SPECK_BYTE_TYPE *)specke->iv;
		specke->carry_bytes = SPECK_BLOCK_SIZE-sz;
		memcpy(specke->carry_block, udst+sz, specke->carry_bytes);
	}
}
