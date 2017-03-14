#include "_stralloc.h"
#include "error.h"
#include "uogetopt.h"
#include "_alloc.h"
#include "fd.h"
#include "fmt.h"
#include "env.h"
#include "byte.h"
#include "bailout.h"
#include "iodptools.h"
#include "coe.h"
#include "pathexec.h"

#define PROGNAME "uxstreamsocket"
const char *progname=PROGNAME;

static const char *opt_mode="0622";

static unsigned long opt_backlog=20;
static int opt_unlink;

uogetopt_t localopts_socket[] = {
{'b', "backlog", UOGO_ULONG, &opt_backlog, 0,
"Allow a backlog of N connections.\n"
"On some systems, n is silently limited to 5.\n"
"On systems supporting SYN cookies, the backlog is\n"
"irrelevant. The default is 20.","N"},
{'m', "mode", UOGO_STRING, &opt_mode, 0,
"Set access rights of the socket to MODE.\n"
"The default is 0622.", "MODE"},
/*2345678901234567890123456789012345678901234567890 */
{'R', "unlink", UOGO_FLAG, &opt_unlink, 1,
"Remove an existing (old) socket.\n"
"Use this if you are sure that no process still\n"
"listings to an existing unix domain socket, or\n"
"if you don't care.",0},
{0, 0, 0, 0, 0, 0, 0}
};
uogetopt_t localopts_data[] = {
{0, 0, 0, 0, 0, 0, 0}
};

static const char *path;
const char *usagestr=
	"usage: " PROGNAME " path-to-socket program [args]";
int skipargs=2;

int doopen(char **argv)
{
	int fd;
	path=argv[1];
	fd=prepare_socketux(path, opt_mode, opt_unlink, 1, 1, opt_backlog,
		opt_print_port);
	return fd;
}
void doiodp(int fd)
{
	iodp_setux(fd, path, "UXSTREAM", opt_symname, "a", opt_backlog);
}

