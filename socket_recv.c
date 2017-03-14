/* reimplementation of socket_recv4 by djb@pobox.com */
/* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"
#include "socklen_t.h"

int 
socket_recv4(int fd,char *buf,int len,char ip[4],uint16 *port)
{
	struct sockaddr_in s_in;
	ssize_t ret;
	socklen_t s_len=sizeof(s_in);
	ret=recvfrom(fd,buf,len,0,(struct sockaddr *) &s_in,&s_len);
	if (ret==-1) return ret;
	if (port)
		socket_ntohs(s_in.sin_port,port);
	if (ip)
  		byte_copy(ip,4,(char *) &s_in.sin_addr);
	return ret;
}
