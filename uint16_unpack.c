/* reimplementation, GPL */
#include "uint16.h"

void uint16_unpack(const char s[2],uint16 *u)
{
	*u=(256*(unsigned char) s[1])+(unsigned char) s[0];
}

void uint16_unpack_big(const char s[2],uint16 *u)
{
	*u=(256*(unsigned char) s[0])+(unsigned char) s[1];
}
