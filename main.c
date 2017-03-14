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
#include <signal.h>
#include <unistd.h>

#include "_stralloc.h"
#include "error.h"
#include "uogetopt.h"
#include "_alloc.h"
#include "fd.h"
#include "str2num.h"
#include "env.h"
#include "str_ulong.h"
#include "bailout.h"
#include "dns.h"
#include "iodptools.h"
#include "ip4.h"
#include "socket.h"
#include "_pathexec.h"
#include "coe.h"

unsigned long opt_uid, opt_gid;
int opt_envuidgid;
long opt_fd=-1;
int verbosity=1;
const char *opt_symname;
const char *opt_chroot;

uogetopt_t myopts[] =
{
{0,"",UOGO_INCLUDE,localopts_socket, 0,0,0},

{0,"",UOGO_TEXT, 0,0, "Child program options:\n",0},
{'F',"fd",UOGO_LONG,&opt_fd, 0,
"Move file handle to file descriptor N.\n"
/*2345678901234567890123456789012345678901234567890 */
"`N' is an integer in the range from 0 to a system\n"
"dependent limit. The default is to not move it.\n"
"Note that moving the socket to file descriptor 2\n"
"may disturb the output of error messages.","N"},
{'C',"chroot",UOGO_STRING,&opt_chroot, 0,
"Chroot to DIR.\n"
"The program will change it's current working\n"
"directory to DIR and then change it's root\n"
"directory to `.'.","DIR"},
{'g',"gid",UOGO_ULONG,&opt_gid,0,
"Group ID to change to.\n"
"The program will switch to that group id after\n"
"the socket has been set up. GID has to be a\n"
"positive integer.","GID"},
{'u',"uid",UOGO_ULONG,&opt_uid,0,
"User ID to change to.\n"
"The program will switch to that user id after\n"
"the socket has been set up. UID has to be a\n"
"positive integer.","UID"},
{'U',"uidgid",UOGO_FLAG,&opt_envuidgid,1,
"The same as -u $UID -g $GID.\n"
"Usually used together with envuidgid.",0},

{0,"",UOGO_TEXT, 0,0, "Data-gathering options:\n",0},
{0,"",UOGO_INCLUDE,localopts_data, 0,0,0},
{'y', "symbolic-name", UOGO_STRING, &opt_symname, 0,
"Use NAME as symbolic file name.\n"
"This is used for $IODP_n_LOCALNAME, and may be\n"
"used by IODP compliant application in an\n"
"application dependant way. The default is to not\n"
"set it.","NAME"},

{0,"",UOGO_TEXT, 0,0, "Verbosity options:\n",0},
/*2345678901234567890123456789012345678901234567890 */
{0,"",UOGO_INCLUDE,localopts_verbose, 0,0,0},
{'q',"quiet",UOGO_FLAG,&verbosity,0,"Quiet. Do not print error messages.",0},
{'Q',0,UOGO_FLAG,&verbosity,1,"Print error messages.\n"
"This is the default.",0},
{'v',"verbose",UOGO_FLAG,&verbosity,2,
"Verbose. Print error and status messages.",0},
{ 0 ,"copyright", UOGO_PRINT|UOGO_HIDDEN,0,0, COPYRIGHT,0},

{0, 0, 0, 0, 0, 0, 0}
};

int
main (int argc, char **argv)
{
	int sock;
	uogetopt_t *opts;
	flag_bailout_log_name=progname;
	uogo_posixmode=1;
	opts=uogetopt_join(myopts);
	uogetopt (progname, PACKAGE, VERSION, &argc, argv, 
		uogetopt_out, usagestr, opts, 0);
	uogetopt_free(opts);
	if (argc<=skipargs)
		xbailout(2,0,usagestr,
			"\nUse the --help option for more information.",0,0);

	sock=doopen(argv);
	if (opt_chroot) {
		if (-1==chdir(opt_chroot))
			xbailout(100,errno,"cannot chdir to ",opt_chroot,0,0);
		if (-1==chroot("."))
			xbailout(100,errno,"cannot chroot to ",opt_chroot,0,0);
	}
	set_ugid(opt_envuidgid,opt_uid,opt_gid);
	if (opt_fd!=-1) {
		move_fd(sock,opt_fd);
		sock=opt_fd;
	}
	doiodp(sock);
	argv+=skipargs;
	pathexec(argv);
	if (verbosity) warning(errno,"cannot run ",argv[0],0,0);
	_exit(111);
}

