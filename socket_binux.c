#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include "byte.h"
#include "socket_ux.h"

int socket_bindux(int fd, const char *path)
{
	struct sockaddr_un sa;
	unsigned int i;

	memset(&sa,0,sizeof(sa));
	byte_zero((char *)&sa,sizeof(sa));
	sa.sun_family = AF_UNIX;

	for (i=0;path[i];i++) {
		if (i>=sizeof(sa.sun_path)) {
#ifdef ENAMETOOLONG
			errno=ENAMETOOLONG;
#else
			errno=EINVAL;
#endif
			return -1;
		}
		sa.sun_path[i]=path[i];
	}
	sa.sun_path[i]=0;
	return bind(fd,(struct sockaddr *) &sa,2+i);
}
