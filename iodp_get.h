struct iodp_get {
	const char *proto;
	int (*handler)(int);
};
int iodp_info(const char **, int fd, const char *what);
int iodp_get (struct iodp_get *);

