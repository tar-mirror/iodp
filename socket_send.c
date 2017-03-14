/* reimplementation of socket_send4 by djb@pobox.com */
/* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"

int socket_send4(int fd,char *buf,int len,char ip[4],uint16 port)
{
	struct sockaddr_in s_in;
	uo_uint16_t np;

	byte_zero((char *)&s_in,sizeof(s_in));
	s_in.sin_family = AF_INET;
	socket_htons(port,&np);
	s_in.sin_port=np;
	byte_copy((char *) &s_in.sin_addr,4,ip);
	return sendto(fd,buf,len,0,(struct sockaddr *) &s_in,sizeof(s_in));
}
