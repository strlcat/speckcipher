#ifndef _SPECK_PRNG_DEFINITIONS_HEADER
#define _SPECK_PRNG_DEFINITIONS_HEADER

#include <stdlib.h>
#include "speckdef.h"

#define SPECK_PRNG_KEY_SIZE SPECK_KEY_SIZE
#define SPECK_PRNG_SIZE_UNIT SPECK_SIZE_UNIT
#define SPECK_PRNG_RANGE(C, T, S, D) (S + C / ((T)~0 / (D - S + 1) + 1))

size_t speck_prng_datasize(void);
void speck_prng_seedkey_r(void *sdata, const void *skey);
void speck_prng_seedkey(const void *skey);
void speck_prng_genrandom_r(void *sdata, void *result, size_t need);
void speck_prng_genrandom(void *result, size_t need);
void speck_prng_seed_r(void *sdata, SPECK_UNIT_TYPE seed);
void speck_prng_seed(SPECK_UNIT_TYPE seed);
SPECK_UNIT_TYPE speck_prng_random_r(void *sdata);
SPECK_UNIT_TYPE speck_prng_random(void);
SPECK_UNIT_TYPE speck_prng_range_r(void *sdata, SPECK_UNIT_TYPE s, SPECK_UNIT_TYPE d);
SPECK_UNIT_TYPE speck_prng_range(SPECK_UNIT_TYPE s, SPECK_UNIT_TYPE d);

#endif
