#ifndef _SPECK_CIPHER_SUPPORTING_HEADER
#define _SPECK_CIPHER_SUPPORTING_HEADER

typedef unsigned long long speck_fsize;

typedef void (*sighandler_t)(int);

#define NOSIZE ((size_t)-1)
#define NOFSIZE ((speck_fsize)-1)

#define DATASIZE 65536
#define TESTTIME 5

size_t xstrlcpy(char *dst, const char *src, size_t size);
#define xzstrlcpy(d, s, sz) do { memset(d, 0, sz); xstrlcpy(d, s, sz); } while(0)
int mhexdump(const void *data, size_t szdata, int hgroup);

#endif
