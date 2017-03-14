#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "iodptools.h"
#include "bailout.h"
#include "str2num.h"
#include "str_ulong.h"
#include "env.h"
#include "str.h"
#include "_dns.h"
#include "socket.h"
#include "socket_ux.h"
#include "error.h"
#include "ip4.h"
#include "byte.h"
#include "_buffer.h"
#include "prot.h"
#include "ndelay.h"
#include "_pathexec.h"
#include "fmt.h"

int
prepare_socketux(const char *path, const char *smode, int delete,
	int stream, int dolisten, int backlog, int flag_print_port)
{
	int sock;
	unsigned long mode;
	int l;
	l=str2ulong(smode,&mode,0);
	if (l==-1 || smode[l])
		xbailout(100,0,"cannot understand mode ",smode,0,0);
	if (stream)
		sock=socket_streamux();
	else
		sock=socket_dgramux();
	if (sock==-1) xbailout(111,errno,"cannot create unix domain socket",0,0,0);
	if (delete)
		unlink(path);
	if (-1==socket_bindux(sock,path))
		xbailout(111,errno,"cannot bind to ",path,0,0);
	if (-1==chmod(path,mode))
		xbailout(111,errno,"cannot fchmod ",path,0,0);
	ndelay_off(sock);
	if (dolisten) 
		if (-1==socket_listen(sock,backlog))
			xbailout(111,errno,"cannot listen on ",path,0,0);

	if (flag_print_port) {
		buffer b;
		char bspace[1024];
		buffer_init(&b,(BUFFER_CAST)write,1,bspace,sizeof bspace);
		buffer_puts(&b,path);
		buffer_puts(&b,"\n");
		buffer_flush(&b);
	}
	return sock;
}

void
iodp_setux(int sock, const char *path, const char *proto,
const char *symname, const char *flags, int backlog)
{
	iodp_set(sock,proto,0,0,path,symname,flags);
    if (backlog) {
		stralloc x=STRALLOC_INIT;
		char lbuf[FMT_ULONG];
		if (!stralloc_cats(&x,"IODP_")) oom();
		if (!stralloc_catulong0(&x,sock,0)) oom();
		if (!stralloc_cats(&x,"_LISTEN")) oom();
		if (!stralloc_0(&x)) oom();
		lbuf[fmt_ulong(lbuf,backlog)]=0;
		if (!pathexec_env(x.s,lbuf)) oom();
	}
}

