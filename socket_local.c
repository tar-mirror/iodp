/* reimplementation of socket_local4 by djb@pobox.com */
/* uwe@ohse.de, 20000521 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"
#include "socklen_t.h"

int socket_local4(int s,char ip[4],uint16 *port)
{
  struct sockaddr_in sa;
  socklen_t len = sizeof(sa);

  if (getsockname(s,(struct sockaddr *) &sa,&len) == -1) return -1;
  byte_copy(ip,4,(char *) &sa.sin_addr);
  socket_ntohs(sa.sin_port,port);
  return 0;
}
