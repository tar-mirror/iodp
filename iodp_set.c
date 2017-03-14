#include "_stralloc.h"
#include "env.h"
#include "fmt.h"
#include "_pathexec.h"
#include "_str.h"
#include "bailout.h"
#include "iodptools.h"

static void
clean(int sock)
{
	char **e=environ;
	int i;
	char nb[FMT_ULONG];
	stralloc x=STRALLOC_INIT;
	if (!stralloc_copys(&x,"IODP_")) oom();
	if (!stralloc_catb(&x,nb,fmt_ulong(nb,sock))) oom();
	if (!stralloc_append(&x,"_")) oom();
	if (!stralloc_0(&x)) oom();

	for (i=0;e[i];i++) {
		if (str_start(e[i],x.s)) {
			int j=0;
			while (e[i][j] && e[i][j]!='=')
				j++;
			if (e[i][j]) {
				e[i][j]='\0';
				if (!pathexec_env(e[i],0)) oom();
				e[i][j]='=';
			}
		}
	}
	stralloc_free(&x);
}

void
iodp_set(int sock, const char *proto, const char *a, const char *sa,
	const char *name, const char *symname, const char *flags)
{
	stralloc x=STRALLOC_INIT;
	char nb[FMT_ULONG];
	int pos;
	char *e;
	clean(sock);

	nb[fmt_ulong(nb,sock)]=0;
	e=env_get("IODP");
		if (!e) {
		if (!pathexec_env("IODP",nb)) oom();
	} else {
		int i;
		int l=str_len(nb);
		for (i=0;e[i];i++) {
			if (str_start(e+i,nb)
				&& (e[i+l]==',' || !e[i+l])) {
				break;
			}
			while (e[i] && e[i]!=',')
				i++;
		}
		if (!e[i]) {
			if (!stralloc_copys(&x,e)) oom();
			if (!stralloc_append(&x,",")) oom();
			if (!stralloc_cats(&x,nb)) oom();
			if (!stralloc_0(&x)) oom();
			if (!pathexec_env("IODP",x.s)) oom();
		}
	}

	if (!stralloc_copys(&x,"IODP_")) oom();
	if (!stralloc_catb(&x,nb,fmt_ulong(nb,sock))) oom();
	if (!stralloc_append(&x,"_")) oom();
	pos=x.len;
	if (!stralloc_cats(&x,"PROTO") || !stralloc_0(&x)) oom();
	if (!pathexec_env(x.s,proto)) oom();
	x.len=pos;
	if (!stralloc_cats(&x,"LOCALADDR1") || !stralloc_0(&x)) oom();
	if (!pathexec_env(x.s,a)) oom();
	x.len=pos;
	if (sa && *sa) {
		if (!stralloc_cats(&x,"LOCALADDR2") || !stralloc_0(&x)) oom();
		if (!pathexec_env(x.s,sa)) oom();
	}
	x.len=pos;
	if (!stralloc_cats(&x,"LOCALNAME") || !stralloc_0(&x)) oom();
	if (!pathexec_env(x.s,name)) oom();
	x.len=pos;
	if (symname) {
		if (!stralloc_cats(&x,"SYMNAME") || !stralloc_0(&x)) oom();
		if (!pathexec_env(x.s,symname)) oom();
		x.len=pos;
	}
	if (!stralloc_cats(&x,"FLAGS") || !stralloc_0(&x)) oom();
	if (!pathexec_env(x.s,flags)) oom();
	x.len=pos;

}

