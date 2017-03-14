#ifndef STRALLOC_H
#define STRALLOC_H

#include "gen_alloc.h"
#define STRALLOC_INIT {0,0,0}

GEN_ALLOC_typedef(stralloc,char,s,len,a)

int stralloc_ready(stralloc *,unsigned int);
int stralloc_readyplus(stralloc *,unsigned int);
int stralloc_copy(stralloc *,stralloc *);
int stralloc_cat(stralloc *,stralloc *);
int stralloc_copys(stralloc *,const char *);
int stralloc_cats(stralloc *,const char *);
int stralloc_copyb(stralloc *,const char *,unsigned int);
int stralloc_catb(stralloc *,const char *,unsigned int);

/* the next one takes a pointer to 1 char */
int stralloc_append(stralloc *,const char *);
int stralloc_starts(stralloc *,const char *);

#define stralloc_0(sa) stralloc_append(sa,"")

int stralloc_catulong0(stralloc *,unsigned long,unsigned int);
int stralloc_catlong0(stralloc *,long,unsigned int);

void stralloc_free(stralloc *);

#define stralloc_catlong(sa,l) (stralloc_catlong0((sa),(l),0))
#define stralloc_catuint0(sa,i,n) (stralloc_catulong0((sa),(i),(n)))
#define stralloc_catint0(sa,i,n) (stralloc_catlong0((sa),(i),(n)))
#define stralloc_catint(sa,i) (stralloc_catlong0((sa),(i),0))

#endif
