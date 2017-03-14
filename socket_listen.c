/* reimplementation of socket_listen by djb@pobox.com */
/* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

int socket_listen(int fd,int backlog)
{
  return listen(fd,backlog);
}
