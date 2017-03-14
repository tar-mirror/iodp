/* reimplementation of socket_tcp by djb@pobox.com */
/* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ndelay.h"
#include "socket.h"

int socket_tcp(void)
{
	int fd;

	fd = socket(AF_INET,SOCK_STREAM,0);
	if (fd == -1) return -1;
	if (ndelay_on (fd) == -1) {
		close (fd);
		return -1;
	}
	return fd;
}
