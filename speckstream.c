#include <string.h>
#include "speckdef.h"
#include "specke.h"

static inline void xor_block(void *dst, const void *src, size_t sz)
{
	const size_t *sx = (const size_t *)src;
	const SPECK_BYTE_TYPE *usx = (const SPECK_BYTE_TYPE *)src;
	size_t *dx = (size_t *)dst;
	SPECK_BYTE_TYPE *udx = (SPECK_BYTE_TYPE *)dst;
	size_t sl = sz;

	for (sl = 0; sl < (sz / sizeof(size_t)); sl++) dx[sl] ^= sx[sl];
	if (sz - (sl * sizeof(size_t))) for (sl *= sizeof(size_t); sl < sz; sl++) udx[sl] ^= usx[sl];
}

void speck_stream_crypt(struct specke_stream *specke, void *out, const void *in, size_t sz)
{
	specke_emit(out, sz, specke);
	xor_block(out, in, sz);
}
