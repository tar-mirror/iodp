/* reimplementation of socket_udp by djb@pobox.com */
/* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> /* close */
#include "ndelay.h"
#include "socket.h"

int 
socket_udp(void)
{
	int fd;

	fd = socket(AF_INET,SOCK_DGRAM,0);
	if (fd == -1) return -1;
	if (ndelay_on(fd) == -1) {
		close(fd);
		return -1;
	}
	return fd;
}
