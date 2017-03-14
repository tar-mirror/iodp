#include "iodptools.h"
#include "uogetopt.h"

int opt_print_port;

uogetopt_t localopts_verbose[] =
{
{'1', 0, UOGO_FLAG, &opt_print_port, 1,
"Print the local port number.\n"
"After preparing to receive connections, print the\n"
"local port number to standard output.",0},
{0, 0, 0, 0, 0, 0, 0}
};

