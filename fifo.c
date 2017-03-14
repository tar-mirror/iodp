#include <sys/types.h>
#include <sys/stat.h>
#include "fifo.h"

int 
fifo_make(const char *fn, int mode) 
{
	return mkfifo(fn,mode);
}
