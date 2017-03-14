#ifndef IPRULES_H
#define IPRULES_H
#include "_stralloc.h"

/* -1: error, errno set */
/*  0: not found */
/*  1: found, result in result */
/* host and info are optional */
int iprulescheck (int fd, const char *ip, const char *host, 
	const char *info, stralloc * result);

#endif
