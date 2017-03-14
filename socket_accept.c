/* reimplementation of socket_accept by djb@pobox.com */
/* uwe@ohse.de, 20000521 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"
#include "socklen_t.h"

int socket_accept4(int fd,char ip[4],uint16 *port)
{
	int nfd;
	struct sockaddr_in s_in;
	socklen_t len=sizeof(s_in);
	nfd=accept(fd,(struct sockaddr *)&s_in,&len);
	if (nfd==-1) return nfd;
	if (port)
		*port=ntohs(s_in.sin_port);
	if (ip)
		byte_copy(ip,4,(char *) &s_in.sin_addr);
	return nfd;
}
