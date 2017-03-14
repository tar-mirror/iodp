#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"
#include "socklen_t.h"

#ifndef TCP_NODELAY
#define TCP_NODELAY 1
#endif

int 
socket_tcpnodelay(int fd)
{
	int val=1;
	return setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,&val,sizeof(val));
}
int 
socket_tcpdelay(int fd)
{
	int val=0;
	return setsockopt(fd,IPPROTO_TCP,TCP_NODELAY,&val,sizeof(val));
}

