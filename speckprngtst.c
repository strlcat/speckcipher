#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "speckprng.h"
#include "specksupport.h"

static char rndata[64];

int main(int argc, char **argv)
{
	SPECK_UNIT_TYPE seed, s, d, rnd;
	unsigned tst;
	size_t x, to;

	if (argc < 2) seed = 12348765;
	else seed = (SPECK_UNIT_TYPE)atol(argv[1]);
	s = 8;
	d = 17;
	to = 16;
	if (argc >= 3) {
		s = (SPECK_UNIT_TYPE)atol(argv[2]);
	}
	if (argc >= 4) {
		d = (SPECK_UNIT_TYPE)atol(argv[3]);
	}
	if (argc >= 5) {
		to = (size_t)atol(argv[4]);
	}

	speck_prng_seed(seed);

	for (x = 0; x < to; x++) {
		rnd = speck_prng_random();
		printf("%08lx ", (long)rnd);
		if (!((x+1) % 8)) putc('\n', stdout);
	}
	putc('\n', stdout);

	for (x = 0; x < to; x++) {
		rnd = speck_prng_range(s, d);
		printf("%lu ", (long)rnd);
		if (!((x+1) % 8)) putc('\n', stdout);
	}
	putc('\n', stdout);

	speck_prng_genrandom(rndata, sizeof(rndata));
	speck_prng_seedkey(rndata);

	speck_prng_genrandom(rndata, sizeof(rndata));
	mhexdump(rndata, sizeof(rndata), 0);
	speck_prng_genrandom(rndata, sizeof(rndata));
	mhexdump(rndata, sizeof(rndata), 0);
	speck_prng_genrandom(rndata, sizeof(rndata)-9);
	mhexdump(rndata, sizeof(rndata)-9, 0);
	putc('\n', stdout);

	return 0;
}
