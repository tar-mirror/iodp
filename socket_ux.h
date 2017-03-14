#ifndef SOCKET_UX_H
#define SOCKET_UX_H

#include "uint16.h"
#include "_stralloc.h"

int socket_streamux(void);
int socket_dgramux(void);
int socket_connectux(int fd, const char *path);
int socket_bindux(int fd, const char *path);
int socket_acceptux (int fd, stralloc *path);
int socket_sendux (int fd, const char *buf, int len, const char *path);
int socket_recvux (int fd, char *buf, int len, stralloc *path);
int socket_peekux (int fd, char *buf, int len, stralloc *path);
int socket_localux (int fd, stralloc * path);
int socket_remoteux (int fd, stralloc * path);

#endif
