/* reimplementation of socket_connected by djb@pobox.com */
/* uwe@ohse.de, 20000521 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "_readwrite.h"
#include "byte.h"
#include "socket.h"
#include "socklen_t.h"

int 
socket_connected(int fd)
{
	struct sockaddr_in sa;
	socklen_t len;
	len = sizeof(sa);
	if (getpeername(fd,(struct sockaddr *) &sa,&len) == -1) {
		char ch;
		(void) read(fd,&ch,1); /* errno set */
		return 0;
	}
	return 1;
}
