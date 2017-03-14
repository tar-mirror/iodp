#include <netdb.h>
#include "iodptools.h"
#include "bailout.h"
#include "str2num.h"
#include "str_ulong.h"
#include "env.h"
#include "_dns.h"
#include "socket.h"
#include "error.h"
#include "_ip4.h"
#include "fmt.h"
#include "byte.h"
#include "_buffer.h"
#include "_readwrite.h"
#include "prot.h"
#include "ndelay.h"
#include "_pathexec.h"

int
prepare_socket4(const char *host, const char *port,
	    int stream, int dolisten, int backlog, int flag_print_port)
{
	unsigned long ul;
	uo_uint16_t po;
	char ip[4];
	int sock;
	ssize_t l;

	if (!*host || (*host=='0' && !host[1])) 
		host="0.0.0.0";

	if (0==ip4_scan(host,ip)) {
		stralloc addresses=STRALLOC_INIT;
		stralloc fqdn=STRALLOC_INIT;
		stralloc tmp=STRALLOC_INIT;
		if (!stralloc_copys(&tmp,host))
			xbailout(111,0,"out of memory",0,0,0);
		if (dns_ip4_qualify(&addresses,&fqdn, &tmp) == -1)
			xbailout(111,errno,
				"fatal: temporarily unable to figure out IP address for ",
				host,0,0);
		if (addresses.len < 4)
			xbailout(100,0,"fatal: no IP address for ",host,0,0);
		byte_copy(ip,4,addresses.s);
		stralloc_free(&addresses);
		stralloc_free(&fqdn);
		stralloc_free(&tmp);
	}
	l=str2ulong (port, &ul, 0);
	if (l==-1 || port[l] || ul >65535) {
		struct servent *serv;
		if (stream)
			serv=getservbyname(port,"tcp");
		else
			serv=getservbyname(port,"udp");
		if (!serv)
			xbailout(111,0,port,": not a valid port number or name",0,0);
		socket_ntohs(serv->s_port,&po);
	} else
		po=ul;

	if (stream)
		sock = socket_tcp();
	else
		sock = socket_udp();
	if (sock < 0)
		xbailout(111,errno,"cannot create socket",0,0,0);
	if (socket_bind4_reuse (sock, ip, po))
		xbailout(111,errno,"cannot bind socket",0,0,0);
	if (socket_local4(sock,ip,&po))
		xbailout(111,errno,"getsockname",0,0,0);
	if (dolisten) 
		if (-1==socket_listen(sock,backlog))
			xbailout(111,errno,"listen",0,0,0);
	ndelay_off(sock);
	if (flag_print_port) {
		char nb[STR_ULONG+1];
		buffer b;
		char bspace[STR_ULONG+1];
		buffer_init(&b,(BUFFER_CAST)write,1,bspace,sizeof bspace);
		str_ulong(nb,po);
		buffer_puts(&b,nb);
		buffer_puts(&b,"\n");
		buffer_flush(&b);
	}
	return sock;
}


void
iodp_set4(int sock, const char *lhost, const char *proto, 
	const char *symname, const char *flags, int backlog)
{
	char la4[4];
	char lipbuf[IP4_FMT];
	char lportbuf[FMT_ULONG];
	uo_uint16_t lp4;
	if (-1==socket_local4(sock,la4,&lp4))  {
		if (verbosity)
			warning(errno,"cannot inquire local socket address",0,0,0);
		_exit(111);
	}
	lipbuf[ip4_fmt(lipbuf,la4)]=0;
	lportbuf[fmt_ulong(lportbuf,lp4)]=0;

	if (!lhost) {
		static stralloc localhoststr=STRALLOC_INIT;
		if (0==dns_name4(&localhoststr,la4))
			if (localhoststr.len) {
				if (!stralloc_0(&localhoststr)) oom();
				lhost=localhoststr.s;
			}
	}
	iodp_set(sock,proto,lipbuf,lportbuf,lhost,symname,flags);
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

