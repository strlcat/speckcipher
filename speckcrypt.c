#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "speckdef.h"
#include "specke.h"
#include "specksupport.h"

static char key[SPECK_KEY_SIZE];
static char srcblk[DATASIZE], dstblk[DATASIZE];
static struct specke_stream specke;
static int will_exit;

static void usage(void)
{
	printf("usage: speckecrypt key srcfile dstfile.\n");
	printf("This program is part of SPECK stream cipher reference.\n");
	exit(1);
}

static void xerror(const char *s)
{
	perror(s);
	exit(2);
}

int main(int argc, char **argv)
{
	int ifd, ofd;
	char *kfname, *infname, *onfname;
	size_t lio, lrem, ldone, lblock, z, n, x, *sx, *dx;
	char *pblk;

	if (argc < 4) usage();
	kfname = argv[1];
	infname = argv[2];
	onfname = argv[3];
	if (!kfname || !infname || !onfname) usage();

	if (!strcmp(kfname, "-")) ifd = 0;
	else {
		ifd = open(kfname, O_RDONLY);
		if (ifd == -1) xerror(kfname);
	}

	read(ifd, key, sizeof(key));
	if (ifd != 0) close(ifd);

	if (!strcmp(infname, "-")) ifd = 0;
	else {
		ifd = open(infname, O_RDONLY);
		if (ifd == -1) xerror(infname);
	}

	if (!strcmp(onfname, "-")) ofd = 1;
	else {
		ofd = creat(onfname, 0666);
		if (ofd == -1) xerror(onfname);
	}

	specke_init(&specke, key);
	will_exit = 0;
	switch (sizeof(size_t)) {
		case 2: n = 1; break;
		case 4: n = 2; break;
		case 8: n = 3; break;
	}
	while (1) {
		if (will_exit) break;
		pblk = srcblk;
		ldone = 0;
		lrem = lblock = sizeof(srcblk);
_ragain:	lio = read(ifd, pblk, lrem);
		if (lio == 0) will_exit = 1;
		if (lio != NOSIZE) ldone += lio;
		else xerror(infname);
		if (lio && lio < lrem) {
			pblk += lio;
			lrem -= lio;
			goto _ragain;
		}

		specke_emit(dstblk, ldone, &specke);
		sx = (size_t *)srcblk; dx = (size_t *)dstblk;
		for (z = 0; z < (ldone >> n); z++) dx[z] ^= sx[z];
		if (ldone - (z << n)) for (x = (z << n); x < ldone; x++) dstblk[x] ^= srcblk[x];

		pblk = dstblk;
		lrem = ldone;
		ldone = 0;
_wagain:	lio = write(ofd, pblk, lrem);
		if (lio != NOSIZE) ldone += lio;
		else xerror(onfname);
		if (lio < lrem) {
			pblk += lio;
			lrem -= lio;
			goto _wagain;
		}
	}
	specke_emit(NULL, 0, &specke);

	close(ifd);
	close(ofd);

	return 0;
}
