/* reimplementation, GPL */

#include "uint32.h"

void 
uint32_unpack(const char s[4],uint32 *u)
{
	const unsigned char *t=s;
	*u=(t[3] << 24) + (t[2] << 16) + (t[1] << 8) + t[0];
}

void 
uint32_unpack_big(const char s[4],uint32 *u)
{
	const unsigned char *t=s;
	*u=(t[0] << 24) + (t[1] << 16) + (t[2] << 8) + t[3];
}
