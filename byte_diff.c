/* reimplementation of byte_diff.c from djb@pobox.com */
/* +prototype, +const */
/* uwe@ohse.de, 2000-05-21 */

#include "byte.h"

int 
byte_diff(const char *s,unsigned int n,const char *t)
{
	for (;;) {
		if (!n) return 0; 
		if (*s != *t) break; 
		++s; ++t; --n;

		if (!n) return 0; 
		if (*s != *t) break; 
		++s; ++t; --n;

		if (!n) return 0; 
		if (*s != *t) break; 
		++s; ++t; --n;

		if (!n) return 0; 
		if (*s != *t) break; 
		++s; ++t; --n;
	}
	return ((int)(unsigned int)(unsigned char) *s)
	   - ((int)(unsigned int)(unsigned char) *t);
}
