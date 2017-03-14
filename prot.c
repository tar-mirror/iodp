#include <sys/types.h>
#include <unistd.h>
#include "prot.h"

static int 
mysetgroups(int gid)
{
	gid_t g=gid;
	return setgroups(1,&g);
}

int prot_gid(int gid)
{
	if (-1==mysetgroups(gid))
		return -1;
	return setgid(gid);
}

int prot_uid(int uid)
{
	return setuid(uid);
}
