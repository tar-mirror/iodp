#ifndef BAILOUT_H
#define BAILOUT_H

extern const char *flag_bailout_log_name;
extern int flag_bailout_log_pid;

void warning(int erno, const char *s1, const char *s2, const char *s3,
        const char *s4);
void bailout(int erno, const char *s1, const char *s2, const char *s3,
        const char *s4);
void oom(void);
void xbailout(int ecode, int erno, const char *s1, const char *s2, 
		const char *s3, const char *s4);

#endif
