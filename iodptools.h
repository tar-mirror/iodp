#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "typesize.h"
#include "uogetopt.h"

extern const char *usagestr;
extern const char *progname;
extern uogetopt_t myopts[];
extern uogetopt_t localopts_socket[];
extern uogetopt_t localopts_data[];
extern uogetopt_t localopts_verbose[];
extern int skipargs;
extern int verbosity;
extern unsigned long opt_uid, opt_gid;
extern int opt_envuidgid;
extern long opt_fd;
extern int verbosity;
extern const char *opt_symname;
extern int opt_print_port;



extern void set_ugid(int flag_envuidgid, unsigned long uid,
	unsigned long gid);
extern void move_fd(int sock, int to);
extern int doopen(char **);
extern void doiodp(int);

int prepare_socket4(const char *host, const char *port, int stream, 
	int dolisten, int backlog, int flag_print_port);
int prepare_socketux(const char *path, const char *smode, int delete,
    int stream, int dolisten, int backlog, int flag_print_port);

void iodp_set4(int sock, const char *lhost, const char *proto,
    const char *symname, const char *flags, int backlog);
void iodp_setux(int sock, const char *path, const char *proto,
	const char *symname, const char *flags, int backlog);
void iodp_set(int sock, const char *proto, const char *a, const char *sa,
    const char *name, const char *symname, const char *flags);



#if 0
extern const char *localhost;
extern const char *opt_rulesfname;
extern int opt_allow_missing_rules;
extern void printstatus(unsigned int childs, unsigned int maxchilds);
extern int eat(void);
extern int eatfd;
int prepare_uxsocket(const char *path, const char *smode, int delete,
        int stream, int dolisten, int backlog);

void child_prepare(int sock, char *ip, uo_uint16_t port);
void child_action(void);
void usage(void);
extern unsigned long opt_uid, opt_gid;
extern int opt_envuidgid;
extern int opt_hostlookup;
extern int opt_paranoia;
extern int opt_print_port;
void discard(int er, const char *s1, const char *s2, const char *ip);
void run(int sock, char **av, const char *proto, const char *path,
	const char *socketname);
#endif

#define COPYRIGHT \
"Copyright (C) 2000 Uwe Ohse\n" \
PACKAGE " comes with NO WARRANTY, to the extent permitted by law.\n" \
"\n" \
"You may distribute unmodified copies of this package. You may distribute\n" \
"modified copies of this package as long as you do not modify files under\n" \
"a copyright not permitting modifications. See the file LICENSES for more\n" \
"information.\n" \
"This package is distributed under the GNU General Public License, with the \n"\
"special exceptions noted above.\n"

#endif
