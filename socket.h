#ifndef SOCKET_H
#define SOCKET_H

#include "uint16.h"

int socket_tcp(void);
int socket_udp(void);

int socket_connect4 (int fd, char *ip, uo_uint16_t port);
int socket_connected (int fd);
int socket_bind4 (int fd, char *ip, uo_uint16_t port);
int socket_bind4_reuse (int fd, char *ip, uo_uint16_t port);
int socket_listen (int fd, int backlog);
int socket_accept4 (int fd, char *ip, uo_uint16_t * port);
int socket_recv4 (int fd, char *buf, int len, char *ip, uo_uint16_t * port);
int socket_peek4 (int fd, char *buf, int len, char *ip, uo_uint16_t * port);
int socket_send4 (int fd, char *buf, int len, char *ip, uo_uint16_t port);
int socket_local4 (int fd, char *ip, uo_uint16_t * port);
int socket_remote4 (int fd, char *ip, uo_uint16_t * port);
void socket_htons (uo_uint16_t hport, uo_uint16_t *nport);
void socket_ntohs (uo_uint16_t nport, uo_uint16_t *hport);
int socket_tcpnodelay(int fd);
int socket_tcpdelay(int fd);
int socket_ipoptionskill(int fd);

void socket_tryreservein(int fd,int size);

#endif
