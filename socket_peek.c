/* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"
#include "socklen_t.h"

int 
socket_peek4(int s,char *buf,int len,char ip[4],uint16 *port)
{
	struct sockaddr_in sa;
	socklen_t dummy = sizeof sa;
	int r;

	r = recvfrom(s,buf,len,MSG_PEEK,(struct sockaddr *) &sa,&dummy);
	if (r == -1) return -1;

	if (ip)
		byte_copy(ip,4,(char *) &sa.sin_addr);
	if (port)
		socket_ntohs(sa.sin_port,port);

	return r;
}
