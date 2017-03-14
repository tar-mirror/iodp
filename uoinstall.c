#include <unistd.h>
#include <stdio.h> /* rename */
#include "error.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include "_buffer.h"
#include "_str.h"
#include "env.h"
#include "str_ulong.h"
#include "str2num.h"
#include "open.h"
#include "seek.h"
#include "_stralloc.h"
#include "bailout.h"
#include "_getln.h"

static char linenobuf[STR_ULONG];
static int chuid_if_created;
static int chgid_if_created;
static int chmod_if_created;
static uid_t uid;
static gid_t gid;
static unsigned long mode;
static stralloc s_type=STRALLOC_INIT;
static stralloc s_name=STRALLOC_INIT;
static stralloc s_uid=STRALLOC_INIT;
static stralloc s_gid=STRALLOC_INIT;
static stralloc s_mode=STRALLOC_INIT;
static stralloc s_proto=STRALLOC_INIT;
static int dochown=1;

static void
do_dir(void)
{
	uid_t u=uid;
	gid_t g=gid;
	int domod=1;
	if (-1==mkdir(s_name.s,mode)) {
		if (errno!=error_exist)
			bailout(errno,"mkdir(",s_name.s,")",0);
		if (chuid_if_created) u=-1;
		if (chgid_if_created) g=-1;
		if (chmod_if_created) domod=0;
	}
	if (dochown && -1==chown(s_name.s,u,g))
		bailout(errno,"chown(",s_name.s,")",0);
	if (domod && -1==chmod(s_name.s,mode))
		bailout(errno,"chmod(",s_name.s,")",0);
}

static void 
do_file(int replace)
{
	int fd1,fd2;
	static stralloc bak=STRALLOC_INIT;
	static stralloc tmp=STRALLOC_INIT;
	buffer i,o;
	char bi[4096];
	char bo[4096];


	if (!replace) {
		struct stat st;
		if (0==stat(s_name.s,&st))
			return;
		if (errno!=error_noent)	
			bailout(errno,"stat(",s_name.s,")",0);
	}

	fd1=open_read(s_proto.s);
	if (fd1==-1) bailout(errno,"open(",s_proto.s,")",0);

	if (!stralloc_copys(&bak,s_name.s)) oom();
	if (!stralloc_cats(&bak,".bak")) oom();
	if (!stralloc_0(&bak)) oom();
	if (!stralloc_copys(&tmp,s_name.s)) oom();
	if (!stralloc_cats(&tmp,".tmp")) oom();
	if (!stralloc_0(&tmp)) oom();

	fd2=open_trunc_mode(tmp.s,mode);
	if (fd2==-1) bailout(errno,"open(",tmp.s,")",0);
	if (dochown && -1==chown(tmp.s,uid,gid))
		bailout(errno,"chown(",s_name.s,")",0);
	if (-1==chmod(tmp.s,mode))
		bailout(errno,"chmod(",tmp.s,")",0);
	buffer_init(&i,(BUFFER_CAST)read,fd1,bi,sizeof(bi));
	buffer_init(&o,(BUFFER_CAST)write,fd2,bo,sizeof(bo));
	for (;;) {
		int l;
		int w;
		char *p;
		l = buffer_feed(&i);
		if (l==-1) bailout(errno,"read(",s_proto.s,")",0);
		if (!l) break;
		p = buffer_peek(&i);
		w = buffer_put(&o,p,l);
		if (l==-1) bailout(errno,"write(",tmp.s,")",0);
		buffer_seek(&i,l);
	}
	if (-1==buffer_flush(&o)) bailout(errno,"write(",tmp.s,")",0);
	if (-1==fsync(fd2)) bailout(errno,"fsync(",tmp.s,")",0);
	close(fd2);
	if (-1==rename(s_name.s,bak.s) && errno!=error_noent)
		bailout(errno,"rename(",s_name.s,")",0);
	if (-1==rename(tmp.s,s_name.s) && errno!=error_noent)
		bailout(errno,"rename(",s_name.s,")",0);
}

static void 
do_subst(int replace)
{
	int fd1,fd2;
	static stralloc bak=STRALLOC_INIT;
	static stralloc tmp=STRALLOC_INIT;
	buffer i,o;
	char bi[4096];
	char bo[4096];

	if (!replace) {
		struct stat st;
		if (0==stat(s_name.s,&st))
			return;
		if (errno!=error_noent)	
			bailout(errno,"stat(",s_name.s,")",0);
	}

	fd1=open_read(s_proto.s);
	if (fd1==-1) bailout(errno,"open(",s_proto.s,")",0);

	if (!stralloc_copys(&bak,s_name.s)) oom();
	if (!stralloc_cats(&bak,".bak")) oom();
	if (!stralloc_0(&bak)) oom();
	if (!stralloc_copys(&tmp,s_name.s)) oom();
	if (!stralloc_cats(&tmp,".tmp")) oom();
	if (!stralloc_0(&tmp)) oom();

	fd2=open_trunc_mode(tmp.s,mode);
	if (fd2==-1) bailout(errno,"open(",tmp.s,")",0);
	if (dochown && -1==chown(tmp.s,uid,gid))
		bailout(errno,"chown(",s_name.s,")",0);
	if (-1==chmod(tmp.s,mode))
		bailout(errno,"chmod(",tmp.s,")",0);
	buffer_init(&i,(BUFFER_CAST)read,fd1,bi,sizeof(bi));
	buffer_init(&o,(BUFFER_CAST)write,fd2,bo,sizeof(bo));

	while (1) {
		char *q;
		int gotat;
		stralloc sa=STRALLOC_INIT;
		if (-1==getln(&i,&sa,&gotat,'@')) 
			bailout(errno,"read(",s_proto.s,")",0);
		if (sa.len==0) break;

		if (!gotat) {
			if (-1==buffer_put(&o,sa.s,sa.len-1)) bailout(errno,"write",0,0,0);
			break; /* last "line" */
		}
		if (-1==buffer_put(&o,sa.s,sa.len)) 
			bailout(errno,"write(",tmp.s,")",0);

		if (-1==getln(&i,&sa,&gotat,'@')) 
			bailout(errno,"read(",s_proto.s,")",0);
		if (sa.len==0) break;
		if (!gotat) break; /* this eats everything since the @ */
		if (sa.len==1) { /* @@ -> @ */
			if (-1==buffer_put(&o,"@",1))
				bailout(errno,"write(",tmp.s,")",0);
			continue;
		}
		sa.s[sa.len-1]=0;
		q=env_get(sa.s);
		if (!q) q=sa.s; /* replace @nonexistant@ with nonexistant */
		if (-1==buffer_puts(&o,q))
		bailout(errno,"write(",tmp.s,")",0);
	}
	if (-1==buffer_flush(&o)) bailout(errno,"write(",tmp.s,")",0);
	if (-1==fsync(fd2)) bailout(errno,"fsync(",tmp.s,")",0);
	close(fd2);
	if (-1==rename(s_name.s,bak.s) && errno!=error_noent)
		bailout(errno,"rename(",s_name.s,")",0);
	if (-1==rename(tmp.s,s_name.s) && errno!=error_noent)
		bailout(errno,"rename(",s_name.s,")",0);
}

static void 
do_text(int replace)
{
	int fd2;
	static stralloc bak=STRALLOC_INIT;
	static stralloc tmp=STRALLOC_INIT;
	buffer o;
	char bo[4096];

	if (!replace) {
		struct stat st;
		if (0==stat(s_name.s,&st))
			return;
		if (errno!=error_noent)	
			bailout(errno,"stat(",s_name.s,")",0);
	}


	if (!stralloc_copys(&bak,s_name.s)) oom();
	if (!stralloc_cats(&bak,".bak")) oom();
	if (!stralloc_0(&bak)) oom();
	if (!stralloc_copys(&tmp,s_name.s)) oom();
	if (!stralloc_cats(&tmp,".tmp")) oom();
	if (!stralloc_0(&tmp)) oom();

	fd2=open_trunc_mode(tmp.s,mode);
	if (fd2==-1) bailout(errno,"open(",tmp.s,")",0);
	if (dochown && -1==chown(tmp.s,uid,gid))
		bailout(errno,"chown(",s_name.s,")",0);
	if (-1==chmod(tmp.s,mode))
		bailout(errno,"chmod(",tmp.s,")",0);

	buffer_init(&o,(BUFFER_CAST)write,fd2,bo,sizeof(bo));
	if (-1==buffer_puts(&o,s_proto.s)
			|| -1==buffer_puts(&o,"\n")
			|| -1==buffer_flush(&o))
		bailout(errno,"write(",tmp.s,")",0);

	if (-1==fsync(fd2)) bailout(errno,"fsync(",tmp.s,")",0);
	close(fd2);
	if (-1==rename(s_name.s,bak.s) && errno!=error_noent)
		bailout(errno,"rename(",s_name.s,")",0);
	if (-1==rename(tmp.s,s_name.s) && errno!=error_noent)
		bailout(errno,"rename(",s_name.s,")",0);
}

static ssize_t 
getstr(buffer *i,stralloc *s,char delim, int alloweof)
{
	while (1) {
		int gotdelim;
		if (-1==getln(i,s,&gotdelim,delim))
			bailout(errno,"read",0,0,0);
		if (s->len==0 && alloweof) return 0;
		if (s->len==0)
			bailout(0,"premature EOF in input line ",linenobuf,0,0);
		if (!gotdelim)
			bailout(0,"missing terminator in input line ",linenobuf,0,0);
		if (s->len==1 && alloweof) {
			stralloc sb=STRALLOC_INIT;
			if (-1==getln(i,&sb,&gotdelim,'\n'))
				bailout(errno,"read",0,0,0);
			stralloc_free(&sb);
			continue;
		}
		s->s[s->len-1]=0;
		return s->len;
	}
}

static void
interpret(stralloc *s)
{
	static stralloc t=STRALLOC_INIT;
	unsigned int i;
	t.len=0;
	for (i=0;i<s->len;i++) {
		unsigned int j;
		char *v;
		if (s->s[i]!='$') {
	      append:
			if (!stralloc_append(&t,&s->s[i])) oom();
			continue;
		}
		for (j=i;j<s->len;j++) {
			if (s->s[j]=='}') 
				break;
		}
		if (j==s->len) goto append;
		if (j-i<3) goto append; /* ${a} */
		if (s->s[i+1]!='{') goto append;
		s->s[j]='\0';
		v=env_get(s->s+i+2);
		if (v) 
			if (!stralloc_cats(&t,v)) oom();
		i=j;
	}
	if (!stralloc_0(&t)) oom();
	if (!stralloc_copy(s,&t)) oom();
}

int 
main(int argc, char **argv)
{
	const char *prefix;
	unsigned long lineno=0;
	const char *progname;
	int off;
	progname=argv[0];
	off=str_rchr(progname,'/');
	if (progname[off])
		progname+=off;
	if (argc!=1)
		bailout(0,"usage: ",progname," <commandfile",0);
	prefix=env_get("prefix");
	if (!prefix)
		bailout(0,"$prefix not set",0,0,0);
	if (env_get("nochown"))
		dochown=0;
	while (1) {
		char *env;
		struct passwd *pwd;
		struct group *grp;
		int i;
		lineno++;
		str_ulong(linenobuf,lineno);
		if (!getstr(buffer_0,&s_type,':',1)) break;
		getstr(buffer_0,&s_name,':',0);
		interpret(&s_name);
		if (s_name.s[0]!='/') {
			stralloc x=STRALLOC_INIT;
			if (!stralloc_copys(&x,prefix)) oom();
			if (!stralloc_cats(&x,"/")) oom();
			if (!stralloc_cats(&x,s_name.s)) oom();
			if (!stralloc_copy(&s_name,&x)) oom();
			if (!stralloc_0(&s_name)) oom();
			stralloc_free(&x);
		}
		getstr(buffer_0,&s_uid,':',0);
		getstr(buffer_0,&s_gid,':',0);
		getstr(buffer_0,&s_mode,':',0);
		getstr(buffer_0,&s_proto,'\n',0);

		chuid_if_created=0;
		i=str_chr(s_uid.s,'@');
		if (s_uid.s[i]) {
			if (str_equal(s_uid.s+i+1,"if-created"))
				chuid_if_created=1;
			else
				bailout(0,"unknown flag in user id ",s_uid.s, 
					" referenced in line ", linenobuf);
			s_uid.s[i++]=0;
		}
		
		chgid_if_created=0;
		i=str_chr(s_gid.s,'@');
		if (s_gid.s[i]) {
			if (str_equal(s_gid.s+i+1,"if-created"))
				chgid_if_created=1;
			else
				bailout(0,"unknown flag in group id ",s_gid.s, 
					" referenced in line ", linenobuf);
			s_gid.s[i++]=0;
		}

		chmod_if_created=0;
		i=str_chr(s_mode.s,'@');
		if (s_mode.s[i]) {
			if (str_equal(s_mode.s+i+1,"if-created"))
				chmod_if_created=1;
			else
				bailout(0,"unknown flag in mode ",s_mode.s, 
					" referenced in line ", linenobuf);
			s_mode.s[i++]=0;
		}

		env=env_get(s_uid.s);
		if (!env) env=s_uid.s;
		pwd = getpwnam(env);
		if (!pwd) {
			unsigned long ul;
			if ((int)strlen(env)!=str2ulong(env,&ul,10)
				|| ul != (unsigned long) (uid_t) ul)
				bailout(0,"unknown user id ",env, " referenced in line ",
					linenobuf);
			uid=ul;
		} else
			uid=pwd->pw_uid;

		env=env_get(s_gid.s);
		if (!env) env=s_gid.s;
		grp = getgrnam(env);
		if (!grp) {
			unsigned long ul;
			if ((int)strlen(env)!=str2ulong(env,&ul,10)
				|| ul != (unsigned long) (gid_t) ul)
				bailout(0,"unknown group id ",env, " referenced in line ",
					linenobuf);
			gid=ul;
		} else
			gid=grp->gr_gid;

		if (str2ulong(s_mode.s,&mode,8) != (int)str_len(s_mode.s)) /* \0 */
			bailout(0,"unparsable mode ",s_mode.s, " in line ",linenobuf);

		if (s_name.len<2) /* \0 */
			bailout(0,"no name in line ",linenobuf,0,0);
		if (s_proto.len>1)
			interpret(&s_proto);
		if (str_equal(s_type.s,"dir"))
			do_dir();
		else if (str_equal(s_type.s,"file"))
			do_file(1);
		else if (str_equal(s_type.s,"-file"))
			do_file(0);
		else if (str_equal(s_type.s,"subst"))
			do_subst(1);
		else if (str_equal(s_type.s,"-subst"))
			do_subst(0);
		else if (str_equal(s_type.s,"text"))
			do_text(1);
		else if (str_equal(s_type.s,"-text"))
			do_text(0);
		else
			bailout(0,"unknown type in line ",linenobuf,0,0);
	}
	_exit(0);
}
