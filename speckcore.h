#ifndef _SPECK_CIPHER_CORE_HEADER
#define _SPECK_CIPHER_CORE_HEADER

#define ROL(x, s, max) ((x << s) | (x >> (-s & (max-1))))
#define ROR(x, s, max) ((x >> s) | (x << (-s & (max-1))))

#define SPECK_EF(x, y, k)				\
	do {						\
		x = ROR(x, 8, SPECK_UNIT_BITS);		\
		x += y;					\
		x ^= k;					\
		y = ROL(y, 3, SPECK_UNIT_BITS);		\
		y ^= x;					\
	} while (0)

#endif
