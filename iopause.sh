#! /bin/sh
#
# poll/emulation decision
# most stuff directly taken from djb@pobox.com
#
cat >conftest.c <<EOF
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>

main()
{
	struct pollfd x;

	x.fd = open("trypoll.c",O_RDONLY);
	if (x.fd == -1) _exit(111);
	x.events = POLLIN;
	if (poll(&x,1,10) == -1) _exit(1);
	if (x.revents != POLLIN) _exit(1);

	/* XXX: try to detect and avoid poll() imitation libraries */

	_exit(0);
}
EOF
./compile.sh -c conftest.c  2>/dev/null >/dev/null
if test $? -ne 0 ; then
cat <<EOF
#ifndef IOPAUSE_H
#define IOPAUSE_H

/* sysdep: -poll */

typedef struct {
	int fd;
	short events;
	short revents;
} iopause_fd;

#define IOPAUSE_READ 1
#define IOPAUSE_WRITE 4

#include "taia.h"

extern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);

EOF
cat >conftest.c <<EOF
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
EOF
./compile.sh -c conftest.c  2>/dev/null >/dev/null
if test $? -eq 0 ; then
cat <<EOF
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#endif
EOF
else
cat <<EOF
#include <sys/types.h>
#include <sys/time.h>
#endif
EOF
fi
else
cat <<EOF
#ifndef IOPAUSE_H
#define IOPAUSE_H

/* sysdep: +poll */
#define IOPAUSE_POLL

#include <sys/types.h>
#include <poll.h>

typedef struct pollfd iopause_fd;
#define IOPAUSE_READ POLLIN
#define IOPAUSE_WRITE POLLOUT

#include "taia.h"

extern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);

#endif
EOF
fi
rm -f conftest.c conftest.o conftest
