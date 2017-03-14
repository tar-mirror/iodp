/*
 * Copyright (C) 2000 Uwe Ohse
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "_stralloc.h"
#include "error.h"
#include "uogetopt.h"
#include "_alloc.h"
#include "fd.h"
#include "str2num.h"
#include "env.h"
#include "byte.h"
#include "fmt.h"
#include "bailout.h"
#include "ip4.h"
#include "socket.h"
#include "pathexec.h"
#include "iodptools.h"
#include "coe.h"

#define PROGNAME "tcpsocket"
const char *progname=PROGNAME;
int skipargs=3;

static const char *localhost;
static int opt_tcpnodelay=0;
static int opt_ipoptkill=1;

static unsigned long opt_backlog=20;

uogetopt_t localopts_socket[] = {
{0,"",UOGO_TEXT, 0,0, "Socket options:\n",0},
{'b', "backlog", UOGO_ULONG, &opt_backlog, 0,
"Allow a backlog of N TCP SYNs.\n"
"On some systems, n is silently limited to 5.\n"
"On systems supporting SYN cookies, the backlog is\n"
"irrelevant. The default is 20.","N"},
{'d',"tcpdelay", UOGO_FLAG, &opt_tcpnodelay, 0,
"Delay sending data for a fraction of a second\n"
"whenever the remote host is responding slowly.\n"
"This is the default, but it may not be in the\n"
"future; if you want it, set it explicitly.",0},
{'D',"tcpnodelay", UOGO_FLAG, &opt_tcpnodelay, 1,
"Never delay sending data; enable TCP_NODELAY.",0},
{'o',"allow-ip-options",UOGO_FLAG,&opt_ipoptkill,0,
"Leave IP options alone.\n"
"If the client is sending packets along an\n"
"IP source route, send packets back along the\n"
"same route.",0},
{'O',"kill-ip-options",UOGO_FLAG,&opt_ipoptkill,1,
"Kill IP options.\n"
"A client can still use source routing to connect\n"
"and to send data, but packets will be sent back\n"
"along the default route.\n"
"This is the default",0},
{0, 0, 0, 0, 0, 0, 0}
};
uogetopt_t localopts_data[] = {
{'l', "localhost", UOGO_STRING, &localhost, 0,
"Use NAME as local host name.\n"
"Do not look up the local host name in the DNS.\n"
"This information is used for log information and\n"
"$IODP_n_LOCALNAME.","NAME"},
{0, 0, 0, 0, 0, 0, 0}
};

const char *usagestr=
"usage: " PROGNAME " [options] host port program [arguments]";

int
doopen(char **argv)
{
	int sock;
	sock=prepare_socket4(argv[1],argv[2],1,1,opt_backlog,opt_print_port);
	if (opt_ipoptkill)
		if (-1==socket_ipoptionskill(sock)) {
			if (verbosity) warning(errno,"cannot disable IP options",0,0,0);
			_exit(111);
		}
	if (opt_tcpnodelay)
		if (-1==socket_tcpnodelay(sock)) {
			if (verbosity) warning(errno,"cannot enable TCPNODELAY",0,0,0);
			_exit(111);
		}
	return sock;
}

void
doiodp(int fd)
{
	iodp_set4(fd,localhost,"TCP4",opt_symname,"a",opt_backlog);
}
