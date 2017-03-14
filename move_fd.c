#include "fd.h"
#include "str2num.h"
#include "str_ulong.h"
#include "bailout.h"
#include "iodptools.h"
#include "error.h"

void
move_fd(int sock, int to)
{
	if (-1==fd_move(to,sock)) {
		char nb[STR_ULONG];
		str_ulong(nb,to);
		xbailout(111,errno,"cannot copy socket to file descriptor ",nb,0,0);
	}
}
