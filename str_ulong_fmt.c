/*
 * Copyright (C) 1999 Uwe Ohse
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * Contact: uwe@ohse.de
 */
#include "str_ulong.h"

size_t
str_ulong_fmt(char *s, unsigned long u, unsigned int base,
	unsigned int len, unsigned char fill)
{
	size_t off=0;
	size_t l=str_ulong_base(0,u,base);
	while(l<len) {
		*s++=fill;
		l++;
		off++;
	}
	return str_ulong_base(s,u,base)+off;
}
