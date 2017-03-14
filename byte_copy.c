/* reimplementation of byte_copy.c from djb@pobox.com */
/* +prototype, +const */
/* uwe@ohse.de, 2000-05-21 */

#include "byte.h"

void 
byte_copy(char *to, unsigned int n,const char *from)
{
	for (;;) {
		if (!n) return; 
		*to++ = *from++; --n;

		if (!n) return; 
		*to++ = *from++; --n;

		if (!n) return; 
		*to++ = *from++; --n;

		if (!n) return; 
		*to++ = *from++; --n;
	}
}
