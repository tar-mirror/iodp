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
#include "byte.h"
#include "_buffer.h"
#include "_readwrite.h"
#include "prot.h"

void
set_ugid(int flag_envuidgid,unsigned long uid, unsigned long gid)
{
	if (flag_envuidgid) {
		char *p;
		p=env_get("GID");
		if (p && 1>str2ulong(p,&gid,10))
			xbailout(100,0,"cannot understand $GID ",p,0,0);
		p=env_get("UID");
		if (p && 1>str2ulong(p,&uid,10))
			xbailout(100,0,"cannot understand $UID ",p,0,0);
	}
	if (gid)
		if (-1==prot_gid(gid)) {
			char nb[STR_ULONG];
			str_ulong(nb,gid);
			xbailout(100,errno,"cannot change GID to ",nb,0,0);
		}
	if (uid) {
		if (-1==prot_uid(uid)) {
			char nb[STR_ULONG];
			str_ulong(nb,uid);
			xbailout(100,errno,"cannot change UID to ",nb,0,0);
		}
	}
}

