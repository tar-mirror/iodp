/* reimplementation of byte_copy.c from djb@pobox.com */
/* +prototype */
/* uwe@ohse.de, 2000-05-21 */

#include "byte.h"

void 
byte_zero(char *s,unsigned int n)
{
	for (;;) {
		if (!n) break; 
		s[--n]=0;

		if (!n) break; 
		s[--n]=0;

		if (!n) break; 
		s[--n]=0;

		if (!n) break; 
		s[--n]=0;
	}
}
