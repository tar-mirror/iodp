#include <stddef.h>
#include "socket.h"
void 
socket_ntohs (uo_uint16_t nport, uo_uint16_t *hport)
{
	unsigned char *p=(char *)&nport;
	*hport=p[0]*256 + p[1];
}

