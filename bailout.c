#include <unistd.h> /* write */
#include "error.h"
#include "bailout.h"
#include "fmt.h"

const char *flag_bailout_log_name;
int flag_bailout_log_pid;

static int 
retry_write(int fd, const char *s, int l)
{
	while (l) {
		int w;
		w=write(fd,s,l);
		if (w==-1 && errno==error_intr)
			continue;
		if (w==-1) break;
		s+=w;
		l-=w;
	}
	return 0;
}

void
warning(int erno, const char *s1, const char *s2, const char *s3,
        const char *s4)
{
#define Y(x) retry_write(2,x,strlen(x))
#define X(x) if (x) Y(x)
	if (flag_bailout_log_name) Y(flag_bailout_log_name);
	if (flag_bailout_log_pid) {
		char nb[FMT_ULONG];
		nb[fmt_ulong(nb,(unsigned long) getpid())]=0;
		Y("[");
		Y(nb);
		Y("]: ");
	} else if (flag_bailout_log_name)
		Y(": ");
	X(s1);
	X(s2);
	X(s3);
	X(s4);
	if (erno) { Y(": "); s1=error_str(erno); X(s1); }
	Y("\n");
#undef X
#undef Y
}
void
bailout(int erno, const char *s1, const char *s2, const char *s3,
	const char *s4)
{
	warning(erno,s1,s2,s3,s4);
	_exit(1);
}
void
xbailout(int ec, int erno, const char *s1, const char *s2, const char *s3,
	const char *s4)
{
	warning(erno,s1,s2,s3,s4);
	_exit(ec);
}
void oom(void)
{
	xbailout(111,0,"out of memory",0,0,0);
}

