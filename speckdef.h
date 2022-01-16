#ifndef _SPECK_CIPHER_DEFINITIONS_HEADER
#define _SPECK_CIPHER_DEFINITIONS_HEADER

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif

/* config block */
#define SPECK_128BITS
/* #define SPECK_BIG_ENDIAN */
#define SPECK_NO_ENDIAN

#include <stddef.h>
#include <stdint.h>
#ifndef SPECK_NO_ENDIAN
#include <sys/param.h>
#else
#undef SPECK_BIG_ENDIAN
#endif

#ifdef SPECK_128BITS
#define SPECK_NR_BLOCK_BITS 128
#define SPECK_NR_KEY_BITS 128
#define SPECK_UNIT_TYPE uint64_t
#ifdef SPECK_BIG_ENDIAN
#define SPECK_SWAP_FUNC htobe64
#else
#define SPECK_SWAP_FUNC htole64
#endif
#else
#define SPECK_NR_BLOCK_BITS 64
#define SPECK_NR_KEY_BITS 64
#define SPECK_UNIT_TYPE uint32_t
#ifdef SPECK_BIG_ENDIAN
#define SPECK_SWAP_FUNC htobe32
#else
#define SPECK_SWAP_FUNC htole32
#endif
#endif

#define SPECK_BYTE_TYPE uint8_t
#define SPECK_NR_BLOCK_UNITS 2
#define SPECK_NR_KEY_UNITS 2
#define SPECK_SIZE_UNIT (sizeof(SPECK_UNIT_TYPE))
#define SPECK_BLOCK_SIZE (SPECK_SIZE_UNIT * SPECK_NR_BLOCK_UNITS)
#define SPECK_KEY_SIZE (SPECK_SIZE_UNIT * SPECK_NR_KEY_UNITS)

#define SPECK_TO_BITS(x) ((x) * 8)
#define SPECK_FROM_BITS(x) ((x) / 8)
#define SPECK_MAX_BITS SPECK_NR_BLOCK_BITS
#define SPECK_UNIT_BITS (SPECK_SIZE_UNIT * 8)

static inline void data_to_words(void *p, size_t l)
{
#ifndef SPECK_NO_ENDIAN
	size_t idx;
	SPECK_UNIT_TYPE *P = p;
	SPECK_UNIT_TYPE t;

	for (idx = 0; idx < (l/sizeof(SPECK_UNIT_TYPE)); idx++) {
		t = SPECK_SWAP_FUNC(P[idx]);
		P[idx] = t;
	}
#endif
}

struct specke_stream;

#define speck_convkey(k) do { data_to_words(k, SPECK_KEY_SIZE); } while (0)

void speck_encrypt_rawblk(SPECK_UNIT_TYPE *O, const SPECK_UNIT_TYPE *I, const SPECK_UNIT_TYPE *K);

void speck_encrypt_block(const void *key, void *out, const void *in);

void speck_stream_crypt(struct specke_stream *specke, void *out, const void *in, size_t sz);

#endif
