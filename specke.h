#ifndef _SPECK_STREAM_CIPHER_DEFS
#define _SPECK_STREAM_CIPHER_DEFS

#include "speckdef.h"

struct specke_stream {
	SPECK_UNIT_TYPE key[SPECK_NR_KEY_UNITS];
	SPECK_UNIT_TYPE iv[SPECK_NR_BLOCK_UNITS];
	SPECK_BYTE_TYPE tmp[SPECK_BLOCK_SIZE];
	size_t tidx;
};

void specke_init_iv(struct specke_stream *specke, const void *key, const void *iv);
void specke_init(struct specke_stream *specke, const void *key);
void specke_emit(void *dst, size_t szdst, struct specke_stream *specke);

#endif
