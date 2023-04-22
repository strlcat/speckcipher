#include <string.h>
#include "specke.h"
#include "speckprng.h"

struct speck_prng_data {
	struct specke_stream specke;
	short init;
};

struct speck_prng_data speck_prng_sdata;

size_t speck_prng_datasize(void)
{
	return sizeof(struct speck_prng_data);
}

void speck_prng_seedkey_r(void *sdata, const void *skey)
{
	SPECK_UNIT_TYPE k[SPECK_NR_KEY_UNITS];
	struct speck_prng_data *rprng = sdata;

	memset(rprng, 0, speck_prng_datasize());
	if (!skey) return;

	memcpy(k, skey, SPECK_KEY_SIZE);
	specke_init(&rprng->specke, k);
	rprng->init = 1;

	memset(k, 0, SPECK_KEY_SIZE);
}

void speck_prng_seedkey(const void *skey)
{
	speck_prng_seedkey_r(&speck_prng_sdata, skey);
}

void speck_prng_genrandom_r(void *sdata, void *result, size_t need)
{
	struct speck_prng_data *rprng = sdata;
	memset(result, 0, need);
	specke_emit(result, need, &rprng->specke);
}

void speck_prng_genrandom(void *result, size_t need)
{
	speck_prng_genrandom_r(&speck_prng_sdata, result, need);
}

void speck_prng_seed_r(void *sdata, SPECK_UNIT_TYPE seed)
{
	SPECK_UNIT_TYPE k[SPECK_NR_KEY_UNITS];
	struct speck_prng_data *rprng = sdata;
	size_t x;

	memset(rprng, 0, speck_prng_datasize());
	for (x = 0; x < SPECK_NR_KEY_UNITS; x++) k[x] = seed;
	specke_init(&rprng->specke, k);
	rprng->init = 1;

	memset(k, 0, SPECK_KEY_SIZE);
}

void speck_prng_seed(SPECK_UNIT_TYPE seed)
{
	speck_prng_seed_r(&speck_prng_sdata, seed);
}

SPECK_UNIT_TYPE speck_prng_random_r(void *sdata)
{
	struct speck_prng_data *rprng = sdata;
	SPECK_UNIT_TYPE r;

	if (!rprng->init) return 0;

	specke_emit(&r, sizeof(r), &rprng->specke);
	return r;
}

SPECK_UNIT_TYPE speck_prng_random(void)
{
	return speck_prng_random_r(&speck_prng_sdata);
}

SPECK_UNIT_TYPE speck_prng_range_r(void *sdata, SPECK_UNIT_TYPE s, SPECK_UNIT_TYPE d)
{
	SPECK_UNIT_TYPE c = speck_prng_random_r(sdata);
	if (d <= s) return s;
	return SPECK_PRNG_RANGE(c, SPECK_UNIT_TYPE, s, d);
}

SPECK_UNIT_TYPE speck_prng_range(SPECK_UNIT_TYPE s, SPECK_UNIT_TYPE d)
{
	return speck_prng_range_r(&speck_prng_sdata, s, d);
}
