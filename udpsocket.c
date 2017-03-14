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

#define PROGNAME "udpsocket"
const char *progname=PROGNAME;
static const char *localhost;
int skipargs=3;

uogetopt_t localopts_socket[] = {
{0, 0, 0, 0, 0, 0, 0}
};
uogetopt_t localopts_data[] = {
{'l', "localhost", UOGO_STRING, &localhost, 0,
"Use NAME as local host name.\n"
"Do not look up the local host name in the DNS.\n"
"This information is used to set the environment\n"
"variable IODP_n_LOCALNAME.","NAME"},
/*2345678901234567890123456789012345678901234567890 */
{0, 0, 0, 0, 0, 0, 0}
};

const char *usagestr=
"usage: " PROGNAME " [options] host port program [arguments]";

int 
doopen(char **argv)
{
	return prepare_socket4(argv[1],argv[2],0,0,0,opt_print_port);
}

void
doiodp(int fd)
{
	iodp_set4(fd,localhost,"UDP4",opt_symname,"rw",0);
}
