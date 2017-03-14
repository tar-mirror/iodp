#include <stdlib.h>
#include <errno.h>
#include "cdb.h"
#include "iprules.h"
/* modelled after rules.c from djb */

static int
onerule (struct cdb *cdb, stralloc *searched, stralloc *found)
{
	unsigned int datalen;
#if 0
write(2,"X:",2); write(2,searched->s,searched->len); write(2,"\n",1); 
#endif
	switch (cdb_find (cdb, searched->s, searched->len)) {
	case -1:
		return -1;
	case 0:
		return 0;
	}

	datalen = cdb_datalen (cdb);
	if (!stralloc_ready(found,datalen)) {errno=ENOMEM; return -1; }
	if (cdb_read (cdb, found->s, datalen, cdb_datapos (cdb)) == -1)
		return -1;
	found->len=datalen;
	return 1;
}

static int
docheck (struct cdb *cdb, const char *ip, const char *host, const char *info,
		 stralloc * result)
{
	int ret;
	stralloc x = {0,0,0};

	if (info) {
		if (!stralloc_copys (&x, info))
			goto nomem;
		if (!stralloc_cats (&x, "@"))
			goto nomem;
		if (!stralloc_cats (&x, ip))
			goto nomem;
		ret = onerule (cdb, &x, result);
		if (ret == -1)
			goto err;
		if (ret)
			goto ok;

		if (host) {
			if (!stralloc_copys (&x, info))
				goto nomem;
			if (!stralloc_cats (&x, "@"))
				goto nomem;
			if (!stralloc_cats (&x, "="))
				goto nomem;
			if (!stralloc_cats (&x, host))
				goto nomem;
			ret = onerule (cdb, &x, result);
			if (ret == -1)
				goto err;
			if (ret)
				goto ok;
		}
	}

	if (!stralloc_copys (&x, ip))
		goto nomem;
	ret = onerule (cdb, &x, result);
	if (ret == -1)
		goto err;
	if (ret)
		goto ok;

	if (host) {
		if (!stralloc_copys (&x, "="))
			goto nomem;
		if (!stralloc_cats (&x, host))
			goto nomem;
		ret = onerule (cdb, &x, result);
		if (ret == -1)
			goto err;
		if (ret)
			goto ok;
	}

	if (!stralloc_copys (&x, ip))
		goto nomem;
	while (x.len > 0) {
		if (ip[x.len - 1] == '.') {
			ret = onerule (cdb, &x, result);
			if (ret == -1)
				goto err;
			if (ret)
				goto ok;
		}
		x.len--;
	}

	if (host) {
		while (*host) {
			if (*host == '.') {
				if (!stralloc_copys (&x, "="))
					goto nomem;
				if (!stralloc_cats (&x, host))
					goto nomem;
				ret = onerule (cdb, &x, result);
				if (ret == -1)
					goto err;
				if (ret)
					goto ok;

			}
			host++;
		}
		if (!stralloc_copys (&x, "="))
			goto nomem;
		ret = onerule (cdb, &x, result);
		if (ret == -1)
			goto err;
		if (ret)
			goto ok;
	}

	x.len=0;
	ret = onerule (cdb, &x, result);
	if (ret == -1)
		goto err;
	if (ret)
		goto ok;

	stralloc_free (&x);
	return 0;
  ok:
	stralloc_free (&x);
	return 1;
  nomem:
    errno=ENOMEM;
  err:
	stralloc_free (&x);
	return -1;
}

int
iprulescheck (int fd, const char *ip, const char *host, const char *info, 
	stralloc * result)
{
	int ret;
	struct cdb cdb;
	int e=errno;
	cdb_init (&cdb, fd);
	ret = docheck (&cdb, ip, host, info, result);
	e=errno;
	cdb_free (&cdb);
	errno=e;
	return ret;
}
