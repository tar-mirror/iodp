#include <stddef.h>
#include "socket.h"
void 
socket_htons (uo_uint16_t hport, uo_uint16_t *nport)
{
	unsigned char *p=(char *)nport;
	p[0]=hport / 256;
	p[1]=hport % 256;
	return;
}

