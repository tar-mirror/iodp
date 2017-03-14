/* reimplementation of byte_chr.c from djb@pobox.com */
/* +prototype, +const */
/* uwe@ohse.de, 2000-05-21 */

#include "byte.h"

unsigned int 
byte_chr(const char *s, unsigned int n, int searched)
{
	char ch=searched;
	const char *p=s;

	for (;;) {
		if (!n) break; 
		if (*p == ch) break; ++p; --n;

		if (!n) break; 
		if (*p == ch) break; ++p; --n;

		if (!n) break; 
		if (*p == ch) break; ++p; --n;

		if (!n) break; 
		if (*p == ch) break; ++p; --n;
	}
	return p - s;
}
