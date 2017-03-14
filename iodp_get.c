#include "_stralloc.h"
#include "env.h"
#include "bailout.h"
#include "_scan.h"
#include "fmt.h"
#include "str.h"
#include "iodp_get.h"

int
iodp_info(const char **res,int fd, const char *what)
{
	char nb[FMT_ULONG];
	stralloc x=STRALLOC_INIT;
	if (!stralloc_copys(&x,"IODP_")) return -1;
	if (!stralloc_catb(&x,nb,fmt_ulong(nb,fd))) return -1;
	if (!stralloc_append(&x,"_")) return -1;
	if (!stralloc_cats(&x,what)) return -1;
	if (!stralloc_0(&x)) return -1;
	*res=env_get(x.s);
	stralloc_free(&x);
	if (*res) 
		return 1;
	return 0;
}
int 
iodp_get(struct iodp_get *descriptors)
{
	char *e=env_get("IODP");
	int of;
	int l;
	int count=0;
	stralloc x=STRALLOC_INIT;
	if (!e || !*e) return 0;
	of=0;
	while (e[of]) {
		unsigned long ul;
		int pos;
		char *p;
		unsigned int i;
		int found;
		l=scan_ulong(e+of,&ul);
		if (l==0) xbailout(100,0,"cannot parse $IODP: ",e,0,0);
		if (!stralloc_copys(&x,"IODP_")) return -1;
		if (!stralloc_catb(&x,e+of,l)) return -1;
		if (!stralloc_append(&x,"_")) return -1;
		pos=x.len;

		of+=l;
		if (e[of])
			if (e[of++]!=',') xbailout(100,0,"cannot parse $IODP: ",e,0,0);

		if (!stralloc_cats(&x,"PROTO") || !stralloc_0(&x)) return -1;
		p=env_get(x.s);
		if (!p || !*p) { 
			warning(0,"$",x.s, " is not set, ignoring file descriptor",0);
			continue;
		}
		found=0;
		for (i=0;descriptors[i].proto;i++) {
			if (str_equal(p,descriptors[i].proto)) {
				int tmp;
				tmp=descriptors[i].handler(ul);
				if (tmp<0)
					return tmp;
				if (tmp) {
					found=1;
					count++;
					break;
				}
			}
		}
		if (!found) 
			warning(0,"unknown protocol ",p," in ",x.s);
	}
	return count;
}

