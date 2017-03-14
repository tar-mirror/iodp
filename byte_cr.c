/* reimplementation of byte_cr.c from djb@pobox.com */
/* +prototype, +const */
/* uwe@ohse.de, 2000-05-21 */

#include "byte.h"

void
byte_copyr (char *to, unsigned int n, const char *from)
{
	for (;;) {
		if (!n--) return;
		to[n]=from[n];

		if (!n--) return;
		to[n]=from[n];

		if (!n--) return;
		to[n]=from[n];

		if (!n--) return;
		to[n]=from[n];
	}
}
