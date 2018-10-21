#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "speckdef.h"
#include "specke.h"
#include "specksupport.h"

static uint64_t encbytes;
static char key[SPECK_KEY_SIZE] = "\a\x76syR_\x98?";
static char data[DATASIZE];
static struct specke_stream specke;
static unsigned do_break;

static void do_stop_sig(int unused)
{
	do_break = 1;
}

int main(void)
{
	specke_init(&specke, key);

	printf("Doing stream encryption loop for next %u seconds...\n", TESTTIME);
	signal(SIGALRM, do_stop_sig);
	alarm(TESTTIME);

	while (1) {
		if (do_break) break;
		specke_emit(data, sizeof(data), &specke);
		encbytes += sizeof(data);
	}
	specke_emit(NULL, 0, &specke);

	printf("Done. Stats: encrypted: %llu, byps: %llu\n", encbytes, encbytes / TESTTIME);

	puts("SPECK stream cipher testing program done.");

	return 0;
}
