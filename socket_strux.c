#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
#include "ndelay.h"
#include "socket_ux.h"

int socket_streamux(void)
{
	int fd;

	fd = socket(AF_UNIX,SOCK_STREAM,0);
	if (fd == -1) return -1;
	if (ndelay_on (fd) == -1) {
		close (fd);
		return -1;
	}
	return fd;
}
