#include "error.h"
#include "uogetopt.h"
#include "fd.h"
#include "str2num.h"
#include "str_ulong.h"
#include "bailout.h"
#include "iodptools.h"
#include "socket.h"
#include "socket_ux.h"
#include "coe.h"

#define PROGNAME "uxdgramsocket"
const char *progname=PROGNAME;
static int opt_unlink;
static const char *opt_mode="0622";
int skipargs=2;

uogetopt_t localopts_socket[] = {
{'R', "unlink", UOGO_FLAG, &opt_unlink, 1,
"Remove an existing (old) socket.\n"
"Use this if you are sure that no process still\n"
"listings to an existing unix domain socket, or\n"
"if you don't care.",0},
{'m', "mode", UOGO_STRING, &opt_mode, 0,
"Set access rights of the socket to MODE.\n"
"The default is 0622.", "MODE"},
{0, 0, 0, 0, 0, 0, 0}
};

uogetopt_t localopts_data[] = {
{0, 0, 0, 0, 0, 0, 0}
};

static const char *path;
const char *usagestr=
"usage: " PROGNAME " path-to-socket program [args]";

int doopen(char **argv)
{
	int fd;
	path=argv[1];
	fd=prepare_socketux(path, opt_mode, opt_unlink, 0, 0, 0, opt_print_port);
	return fd;
}
void doiodp(int fd)
{
	iodp_setux(fd, path, "UXDGRAM", opt_symname, "rw", 0);
}

