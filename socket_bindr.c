/* reimplementation of socket_bind4_reuse and socket_tryreservein
 * by djb@pobox.com */
/* uwe@ohse.de, 20000521 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"

int 
socket_bind4_reuse(int fd,char ip[4],uint16 port)
{
  int opt = 1;
  setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof opt);
  return socket_bind4(fd,ip,port);
}

void socket_tryreservein(int fd,int size)
{
  while (size >= 1024) {
    if (setsockopt(fd,SOL_SOCKET,SO_RCVBUF,&size,sizeof size) == 0) return;
    size -= (size >> 5);
  }
}
