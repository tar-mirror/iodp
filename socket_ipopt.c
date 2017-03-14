#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

#ifndef IP_OPTIONS
#define IP_OPTIONS 1
#endif
int 
socket_ipoptionskill(int fd)
{
	return setsockopt(fd,IPPROTO_IP,IP_OPTIONS,0,0);
}

