/* echoserver:
 * a superfluous example of an IODP service.
 * implements:
 * - TCP/IPv4 echo / discard service
 * - UDP/IPv4 echo / discard service
 * - some kind of size limit.
 * - some kind access control.
 */
#include "iopause.h"
#include "_alloc.h"
#include "fmt.h"
#include "iodp_get.h"
#include "bailout.h"
#include "taia.h"
#include "error.h"
#include "_readwrite.h"
#include "_ip4.h"
#include "byte.h"
#include "close.h"
#include "socket.h"
#include "_sig.h"
#include "_scan.h"
#include "open.h"
#include "_stralloc.h"
#include "str.h"
#include "strhash.h"
#include "uogetopt.h"
#include "iprules.h"
#include "dns.h"

#define TEMP_IO_ERROR(x) \
        ((x)==error_again || (x)==error_wouldblock || (x)==error_intr)

#define INITIAL_SOCKET 1

#define STATE_UNUSED 0
#define STATE_ACCEPT 1
#define STATE_CONNECTED 2
#define STATE_UDP 3

struct slot {
	int state;
	unsigned int buflen;
	int initial;
	char *buf;
	unsigned long received;
	unsigned long sent;
	struct taia last_active;
	char ip4[4];

	unsigned long limit;
};
static struct slot *slot;
static iopause_fd * io;
static stralloc * sa;
static struct taia now_taia;
static struct tai now_tai;

static int flag_discard;
static unsigned long flag_timeout=60;
static unsigned long flag_interval=900;  /* 15 minutes */
static unsigned long flag_cdbtimeout=60; 
static char *flag_rulesfile;
static unsigned long flag_maxslots=30;

static unsigned int buflen=4096;
static int slotcount;
static strhash_t hash[2];

static int
doregister(int fd, int state, int initial, const char *desc, char *ip4) 
{
	int i;
	for (i=0;i<(int)flag_maxslots;i++) {
		if (slot[i].state==STATE_UNUSED)
			break;
	}
	if (i==(int)flag_maxslots)
		xbailout(111,0,"internal error: out of available slots",0,0,0);
	io[i].fd=fd;
	io[i].events=IOPAUSE_READ;
	io[i].revents=0;
	slot[i].state=state;
	slot[i].buflen=0;
	slot[i].initial=initial;
	slot[i].received=0;
	slot[i].sent=0;
	if (ip4)
		byte_copy(slot[i].ip4,4,ip4);
	else
		byte_zero(slot[i].ip4,4);
	if (!stralloc_copys(&sa[i],desc)) oom();
	if (!stralloc_0(&sa[i])) oom();
	return i;
}
static void 
unregister(int s, const char *what)
{
	static stralloc t;
	char u[FMT_ULONG];
	unsigned long limit;
	if (!stralloc_copys(&t,sa[s].s)) oom();
	if (!stralloc_cats(&t,": ")) oom();
	if (!stralloc_cats(&t,what)) oom();
	if (!stralloc_cats(&t,": ")) oom();
	if (!stralloc_catb(&t,u,fmt_ulong(u,slot[s].received))) oom();
	if (!stralloc_cats(&t,"/")) oom();
	if (!stralloc_catb(&t,u,fmt_ulong(u,slot[s].sent))) oom();
	if (!stralloc_cats(&t," bytes received/sent. limit is ")) oom();
	limit=slot[s].limit;
	if (!limit) {
		if (!stralloc_cats(&t,"unlimited")) oom();
	} else {
		if (!stralloc_catb(&t,u,fmt_ulong(u,limit))) oom();
	}
	if (!stralloc_0(&t)) oom();
	warning(0,t.s,0,0,0);
	if (slot[s].initial)
		xbailout(100,0,"initial socket closedd",0,0,0);
	close(io[s].fd);
	if (s!=slotcount-1) {
		char *b=slot[s].buf;
		io[s]=io[slotcount-1];
		slot[s]=slot[slotcount-1];
		byte_copy(b,buflen,slot[slotcount-1].buf);
		if (!stralloc_copy(&sa[s],&sa[slotcount-1])) oom();
	}
	slot[slotcount-1].state=STATE_UNUSED;
	slotcount--;
}

static int
handle_iodp_udp4(int fd) 
{
	char nb[FMT_ULONG];
	unsigned int i;
	int read_ok=0;
	int write_ok=0;
	const char *flags;
	const char *name;
	nb[fmt_ulong(nb,fd)]=0;

	switch(iodp_info(&flags,fd,"FLAGS"))
	{
	case -1: oom();
	case 0:  return 0;
	}
	for (i=0;flags[i];i++) {
		switch(flags[i]) {
		case 'r': read_ok=1; break;
		case 'w': write_ok=1; break;
		}
	}
	switch(iodp_info(&name,fd,"SYMNAME")) {
	case -1: oom ();
	case 0: 
		switch(iodp_info(&name,fd,"LOCALNAME")) {
		case -1: oom ();
		case 0: 
			name="unknown";
		}
	}
	if (!(write_ok || flag_discard) || !read_ok)
		return 0;
	doregister(fd,STATE_UDP,INITIAL_SOCKET,name,0);
	return 1;
}
static int
handle_iodp_tcp4(int fd) 
{
	char nb[FMT_ULONG];
	unsigned int i;
	int read_ok=0;
	int write_ok=0;
	int accept_ok=0;
	int connected=0;
	const char *flags;
	const char *name;
	nb[fmt_ulong(nb,fd)]=0;

	switch(iodp_info(&flags,fd,"FLAGS"))
	{
	case -1: oom();
	case 0:  return 0;
	}
	for (i=0;flags[i];i++) {
		switch(flags[i]) {
		case 'r': read_ok=1; break;
		case 'w': write_ok=1; break;
		case 'a': accept_ok=1; break;
		case 'C': connected=1; break;
		}
	}
	switch(iodp_info(&name,fd,"SYMNAME")) {
	case -1: oom ();
	case 0: 
		switch(iodp_info(&name,fd,"LOCALNAME")) {
		case -1: oom ();
		case 0: 
			name="unknown";
		}
	}
	if (connected) {
		if (!(write_ok || flag_discard) || !read_ok)
			return 0;
		doregister(fd,STATE_CONNECTED,INITIAL_SOCKET,name,0);
	} else if (!accept_ok)
		return 0;
	doregister(fd,STATE_ACCEPT,INITIAL_SOCKET,name,0);
	return 1;
}
	
static struct iodp_get prototab[]=
{	
	{ "UDP4", handle_iodp_udp4 },
	{ "TCP4", handle_iodp_tcp4 },
	{ 0,0}
};

static unsigned long
getacco(char *ip4)
{
	int x;
	char *c;
	unsigned long totalbytes=0;
	uo_uint32_t dl;
	x=strhash_lookup(&hash[0],ip4,4,&c,&dl);
	if (x==-1) xbailout(111,0,"internal state corrupted",0,0,0);
	if (x) {
		unsigned int l;
		l=scan_ulong(c,&totalbytes);
		if (l==0 || c[l]) xbailout(100,0,"internal state corrupted",0,0,0);
	}

	x=strhash_lookup(&hash[1],ip4,4,&c,&dl);
	if (x==-1) xbailout(111,0,"internal state corrupted",0,0,0);
	if (x) {
		unsigned int l;
		unsigned long ul;
		l=scan_ulong(c,&ul);
		if (l==0 || c[l]) xbailout(100,0,"internal state corrupted",0,0,0);
		totalbytes+=ul;
	}
	return totalbytes;
}

static void
doinit(void)
{
	struct slot *s;
	iopause_fd *f;
	char *b;
	int i;
	s=(struct slot *) alloc(sizeof(*s) * flag_maxslots);
	if (!s) oom();
	f=(iopause_fd *) alloc(sizeof(*f) * flag_maxslots);
	if (!f) oom();
	b=alloc(buflen * flag_maxslots);
	if (!b) oom();
	sa=(stralloc *) alloc(sizeof (*sa) * flag_maxslots);
	if (!b) oom();
	for (i=0;i<(int) flag_maxslots;i++) {
		f[i].fd=0;
		f[i].events=0;
		s[i].state=STATE_UNUSED;
		s[i].buflen=0;
		s[i].buf=b;
		sa[i].s=0;
		b+=buflen;
	}
	slot=s;
	io=f;
	if (-1==strhash_create(&hash[0],1,256,strhash_hash)) oom();
	if (-1==strhash_create(&hash[1],1,256,strhash_hash)) oom();
}

static int
doacco(char *ip4, int bytes)
{
	int x;
	char d[FMT_ULONG];
	char *c;
	unsigned long long totalbytes;
	uo_uint32_t dl;
	x=strhash_lookup(&hash[0],ip4,4,&c,&dl);
	if (x==-1) xbailout(111,0,"internal state corrupted",0,0,0);
	if (x==0) {
		unsigned int l;
		l=fmt_ulong(d,bytes);
		d[l++]=0;
		totalbytes=bytes;
		x=strhash_enter(&hash[0],1,ip4,4,1,d, l);
		if (x==-1) oom();
	} else {
		unsigned int l;
		unsigned long ul;
		l=scan_ulong(c,&ul);
		if (l==0 || c[l]) xbailout(100,0,"internal state corrupted",0,0,0);
		ul+=bytes;
		l=fmt_ulong(d,ul);
		d[l++]=0;
		totalbytes=ul;
		x=strhash_change(&hash[0],1,d,l);
		if (x==-1) oom();
	}

	x=strhash_lookup(&hash[1],ip4,4,&c,&dl);
	if (x==-1) xbailout(111,0,"internal state corrupted",0,0,0);
	if (x) {
		unsigned int l;
		unsigned long ul;
		l=scan_ulong(c,&ul);
		if (l==0 || c[l]) xbailout(100,0,"internal state corrupted",0,0,0);
		totalbytes+=ul;
	}
	return 1;
}

static void
var_value(int s, char *vv)
{
	unsigned int i;
	unsigned int l;
	unsigned long val;
	i=str_chr(vv,'=');
	if (!vv[i]) return;
	vv[i++]=0;
	l=scan_ulong(vv+i,&val);
	if (l==0 || vv[i+l]) return;
	if (str_equal(vv,"limit")) slot[s].limit=val;
}

static int
accesscheck(int slt, char *ip)
{
	static stralloc out;
	const char *p;
	static int fd=-1;
	static struct taia timeout;
	unsigned int i,j;
	char ipstr[IP4_FMT];

	slot[slt].limit=0;

	ipstr[ip4_fmt(ipstr,ip)]=0;
	if (!flag_rulesfile)
		return 1; /* allow */
	if (-1==dns_name4(&out,ip)) {
		if (errno==error_nomem) oom();
		p=0;
	} else {
		if (!stralloc_0(&out)) oom();
		p=out.s;
	}
	if (fd!=-1) {
		if (taia_less(&timeout,&now_taia)) {
			close(fd);
			fd=-1;
		}
	}
	if (fd==-1) {
		fd=open_read(flag_rulesfile);
		if (fd==-1)
			xbailout(100,errno,"failed to open ",flag_rulesfile,0,0);

		taia_uint(&timeout,flag_cdbtimeout);
		taia_add(&timeout,&now_taia,&timeout);
		taia_now(&timeout);
	}

	switch(iprulescheck(fd, ipstr, p, 0, &out)) {
	case -1: xbailout(111,errno,"iprulescheck failed",0,0,0);
	case 0: return 0; /* not found */
	}

	i=0;
	while (1) {
		j=i+byte_chr(out.s+i,out.len-i,0);
		if (j==out.len) 
			break;
		if (out.s[i]=='D') return 0;
		else if (out.s[i]=='+') {
			/* VAR=VALUE */
			var_value(slt,out.s+i+1);
		}
		i=j+1;

	}
	return 1;
}

static void
dowrite(int s)
{
	int x;
	x=write(io[s].fd,slot[s].buf,slot[s].buflen);
	if (x==-1) {
		if (TEMP_IO_ERROR(errno))
			return;
		warning(errno,"failed to write to ",sa[s].s,0,0);
		unregister(s,"write error");
		return;
	}
	if (x)	 {
		slot[s].last_active=now_taia;
		slot[s].sent+=x;
	}
	if (slot[s].buflen == (unsigned int) x) {
		slot[s].buflen=0;
		io[s].events=IOPAUSE_READ;
		return;
	}
	byte_copy(slot[s].buf,slot[s].buflen-x,slot[s].buf+x);
	slot[s].buflen-=x;
	return;
}

static void
doread(int s)
{
	int x;
	int y;
	x=read(io[s].fd,slot[s].buf,buflen);
	if (x==-1) {
		if (TEMP_IO_ERROR(errno))
			return;
		warning(errno,"failed to read from ",sa[s].s,0,0);
		unregister(s,"read error");
		return;
	}
	if (x==0) {
		unregister(s,"closed");
		return;
	}
	slot[s].last_active=now_taia;
	slot[s].received+=x;
	if (!doacco(slot[s].ip4,x))
		return;
	if (slot[s].limit)
		if (getacco(slot[s].ip4) > slot[s].limit) {
			unregister(s,"limit exceeded");
			return;
		}
	if (flag_discard)
		return;

	y=write(io[s].fd,slot[s].buf,x);
	if (x==-1) {
		if (!TEMP_IO_ERROR(errno)) {
			warning(errno,"failed to write to ",sa[s].s,0,0);
			unregister(s,"write error");
			return;
		}
		slot[s].buflen=x;
		io[s].events=IOPAUSE_WRITE;
	} else if (x!=y) {
		byte_copy(slot[s].buf,x-y,slot[s].buf+y);
		slot[s].buflen=x-y;
		io[s].events=IOPAUSE_WRITE;
		slot[s].sent+=y;
	} else
		slot[s].sent+=y;
}

static void
doudp(int s)
{
	int x;
	int y;
	char ip[4];
	uo_uint16_t port;
	x=socket_recv4 (io[s].fd, slot[s].buf, buflen, ip, &port);
	if (x==-1) {
		if (TEMP_IO_ERROR(errno))
			return;
		warning(errno,"failed to receive from ",sa[s].s,0,0);
		unregister(s,"receive error");
		return;
	}
	if (x==0) {
		unregister(s,"closed");
		return;
	}
	if (!doacco(ip,x))
		return;
	slot[s].limit=0;
	if (!accesscheck(s,ip)) 
		return;
	if (slot[s].limit)
		if (getacco(ip) > slot[s].limit)
			return;
	if (flag_discard)
		return;

	y=socket_send4 (io[s].fd, slot[s].buf,x, ip, port);
	if (x==-1) {
		if (!TEMP_IO_ERROR(errno)) {
			warning(errno,"failed to write to ",sa[s].s,0,0);
			unregister(s,"write error");
			return;
		}
	}
}


static void
doaccept(int s)
{
	int x;
	char ip[4];
	char str[IP4_FMT+20];
	unsigned int l;
	int slt;
	uo_uint16_t port;
	x=socket_accept4(io[s].fd,ip,&port);
	if (x==-1) {
		if (TEMP_IO_ERROR(errno))
			return;
		warning(errno,"failed to accept on ",sa[s].s,0,0);
		return;
	}
	l=ip4_fmt(str,ip);
	str[l++]=':';
	str[l+fmt_ulong(str+l,port)]=0;

	if (slotcount == (int) flag_maxslots) {
		warning(errno,"refused connect from ",str,": no free slots",0);
		close(x);
		return;
	}
	warning(0,"connect from ",str,0,0);
	slt=doregister(x, STATE_CONNECTED, 0,str, ip);
	slotcount++;
	if (!accesscheck(slt, ip)) {
		warning(errno,"refused connect from ",str,": access check failed",0);
		close(x);
		unregister(slt,"access check failed");
		return;
	}
	if (slot[slt].limit && getacco(ip) >= slot[slt].limit) {
		unregister(slt,"limit exceeded");
		close(x);
		return;
	}
}

static uogetopt_t opts[]={
{'d', "discard", UOGO_FLAG, &flag_discard, 1,
 "Discard incoming packets.\n"
 "Do not echo them: Emulate the discard service.",0},
{'i', "interval", UOGO_ULONG, &flag_interval, 0,
 "Interval length in seconds.\n"
 "Accounting data is kept for two time periods of the\n"
 "given length. Default ist 900 seconds (15 minutes).","SECONDS"},
{'r', "rules", UOGO_STRING, &flag_rulesfile, 0,
 "Name of the access control CDB file.\n"
 "The file may be compiled with tcprules.","FILENAME"},
{'s', "slots", UOGO_ULONG, &flag_maxslots, 0,
 "Maximum number of open sockets.\n"
 "No more than NUMBER sockets will be opened. Note\n"
 "that this number includes _all_ sockets, including\n"
 "the server sockets","NUMBER"},
{'t', "timeout", UOGO_ULONG, &flag_timeout, 0,
 "Timeout.\n"
 "Close connections after than many seconds of inactivity.","SECONDS"},
{0,0,0,0,0,0,0}
};

int main(int argc, char **argv)
{
	int i;
	struct taia then_interval;

	uogetopt("echoserver",PACKAGE,VERSION,&argc, argv,
		uogetopt_out, "usage: echoserver [options]", opts, 0);

	doinit();
	sig_ignore(sig_pipe);


	slotcount=iodp_get(prototab);
	if (slotcount==0)
		xbailout(100,0, "No IODP environment found. Did you create any socket?",
			0,0,0);
	if (slotcount==-1)
		xbailout(100,0,"Error parsing IODP environment.",
			0,0,0);
	taia_now(&now_taia);
	taia_uint(&then_interval,flag_interval);
	taia_add(&then_interval,&now_taia,&then_interval);
	while (1) {
		struct taia then_taia;

		taia_uint(&then_taia,flag_timeout);
		taia_add(&then_taia,&now_taia,&then_taia);

#if 0
for (i=0;i<slotcount;i++) {
	printf("slot %d fd %d state %d\n",i,io[i].fd,slot[i].state);
}
fflush(0);
#endif
		iopause(io,slotcount,&then_taia,&now_taia); 
		taia_now(&now_taia);
		taia_tai(&now_taia, &now_tai);

		for (i=slotcount-1;i>=0;i--) {
			if (io[i].revents & IOPAUSE_WRITE) {
				dowrite(i);
			} else if (io[i].revents) {
				switch (slot[i].state) {
				case STATE_ACCEPT: doaccept(i); break;
				case STATE_UDP: doudp(i); break;
				case STATE_CONNECTED: doread(i); break;
				}
			}
			if (!slot[i].initial) {
				taia_uint(&then_taia,flag_timeout);
				taia_add(&then_taia,&slot[i].last_active,&then_taia);
				if (taia_less(&then_taia,&now_taia))
					unregister(i,"timed out");
			}
		}
		if (taia_less(&then_interval,&now_taia)) {
			taia_uint(&then_interval,flag_interval);
			taia_add(&then_interval,&now_taia,&then_interval);
			strhash_destroy(&hash[1]);
			hash[1]=hash[0];
			if (-1==strhash_create(&hash[0],1,256,strhash_hash)) oom();
		}
	}
	exit(0);
}
