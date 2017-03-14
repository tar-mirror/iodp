#include "_stralloc.h"
#include "error.h"
#include "uogetopt.h"
#include "_alloc.h"
#include "fd.h"
#include "str2num.h"
#include "byte.h"
#include "fmt.h"
#include "bailout.h"
#include "pathexec.h"
#include "iodptools.h"
#include "fifo.h"
#include "_str.h"
#include "seek.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define PROGNAME "file-iodp"
const char *progname=PROGNAME;

static const char *opt_mode;
const char *opt_symname;
static int opt_unlink;
static int opt_rdwr=O_RDWR;
static int opt_append=0;
static int opt_create=0;
static int opt_excl=0;
static int opt_trunc=0;
static int opt_fifo=0;
static long opt_reuse=-1;

uogetopt_t localopts_socket[] = {
{0,"",UOGO_TEXT, 0,0, "File options:\n",0},
{'r',"rdonly",UOGO_FLAG,&opt_rdwr,O_RDONLY,
"Open the file read-only.",0},
{'w',"wronly",UOGO_FLAG,&opt_rdwr,O_WRONLY,
"Open the file write-only.",0},
{'a',"append",UOGO_FLAG,&opt_append,O_APPEND,
"Append to end of file.\n"
"Do not truncate.",0},
{'c',"create",UOGO_FLAG,&opt_create,O_CREAT,
"Create new file, if needed.",0},
{'e',"exclusive",UOGO_FLAG,&opt_excl,O_EXCL,
"Fail if file already exists.",0},
{'t',"truncate",UOGO_FLAG,&opt_trunc,O_TRUNC,
"Truncate file.",0},
{'f',"fifo",UOGO_FLAG,&opt_fifo,1,
"Create fifo, not file.",0},
{'m', "mode", UOGO_STRING, &opt_mode, 0,
"Set access rights of the file to MODE.\n"
"The default is 0644 for files and 622 for fifos.", "MODE"},
{'n',"reuse",UOGO_LONG,&opt_reuse,1,
"Reuse existing file handle FD.\n"
"Do not open another file","FD"},
{'R',"unlink",UOGO_FLAG,&opt_unlink,1,
"Remove existing file.",0},
{0, 0, 0, 0, 0, 0, 0}
};


uogetopt_t localopts_data[] = {
{'y', "symbolic-name", UOGO_STRING, &opt_symname, 0,
"Use NAME as symbolic file name.\n"
"This is used for $IODP_n_LOCALNAME, and may be\n"
"used by IODP compliant application in an\n"
"application dependant way. The default is to not\n"
"set it.","NAME"},
{0, 0, 0, 0, 0, 0, 0}
};

uogetopt_t localopts_verbose[] =
{
{0, 0, 0, 0, 0, 0, 0}
};

static const char *path;
const char *usagestr=
"usage: " PROGNAME " filename program [args]";
int skipargs=2;

int doopen(char **argv)
{
	int fd;
	int mode;
	unsigned long acmode;
	path=argv[1];
	if (!opt_mode) {
		if (opt_fifo)
			opt_mode="0622";
		else
			opt_mode="0644";
	}
	if (opt_reuse != -1)
		return opt_reuse;
	if (str2ulong(opt_mode,&acmode,8)!=(int) str_len(opt_mode))
		xbailout(2,0,"can't understand access mode ",opt_mode,0,0);
	if (opt_unlink)
		if (-1==unlink(path) && errno!=error_noent)
			bailout(errno,"cannot unlink ",path,0,0);
	if (opt_fifo) {
		int ret;
		ret=fifo_make(path,acmode);
		if (ret==-1) bailout(errno,"cannot open fifo ",path,0,0);
	}

	mode=opt_rdwr | opt_append | opt_create | opt_trunc | opt_excl 
		| O_NONBLOCK;
	fd=open(path,mode,acmode);
	if (fd==-1) bailout(errno,"cannot open ",path,0,0);

	if (-1==fchmod(fd,acmode))
		bailout(errno,"fchmod ",path,0,0);
	return fd;
}
void doiodp(int fd)
{
	const char *m;
	stralloc ma=STRALLOC_INIT;
	off_t x,y;
	if (opt_fifo)
		m="rw";
	else if (opt_rdwr==O_RDONLY)
		m="r";
	else if (opt_rdwr==O_WRONLY)
		m="w";
	else
		m="rw";
	if (!stralloc_copys(&ma,m)) oom();
	if (isatty(fd)) {
		if (!stralloc_cats(&ma,"T")) oom();
	}
	else {
		x=seek_cur(fd);
		if (x!=-1) {
			y=seek_begin(fd);
			if (y!=-1) {
				y=seek_set(fd,x);
				if (y==-1) xbailout(111,errno,"lseek",0,0,0);
				if (!stralloc_cats(&ma,"S")) oom();
			}
		}
	}
	if (!stralloc_0(&ma)) oom();

	iodp_set(fd,"FILE",0,0,path,opt_symname,ma.s);
}

