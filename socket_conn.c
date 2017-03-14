/* reimplementation of socket_connect4 by djb@pobox.com */
 /* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "byte.h"
#include "socket.h"

int socket_connect4(int fd,char ip[4],uint16 port)
{
	struct sockaddr_in sa;
	uo_uint16_t np;

	memset(&sa,0,sizeof(sa));
	byte_zero((char *)&sa,sizeof(sa));
	sa.sin_family = AF_INET;

	byte_copy((char *)&sa.sin_addr,4,ip);
	socket_htons(port,&np);
	sa.sin_port=np;

	return connect(fd,(struct sockaddr *) &sa,sizeof sa);
}
