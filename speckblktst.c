#include <string.h>
#include "speckdef.h"
#include "specksupport.h"

static char data[SPECK_BLOCK_SIZE] = " made it equival";
static char key[SPECK_KEY_SIZE] = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";

int main(int argc, char **argv)
{
	if (argc >= 2) xzstrlcpy(key, argv[1], sizeof(key));
	if (argc >= 3) xzstrlcpy(data, argv[2], sizeof(data));

	mhexdump(data, SPECK_BLOCK_SIZE, SPECK_BLOCK_SIZE);
	speck_encrypt_block(key, data, data);
	mhexdump(data, SPECK_BLOCK_SIZE, SPECK_BLOCK_SIZE);

	return 0;
}
