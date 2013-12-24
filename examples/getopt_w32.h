#ifndef __GETOPT_W32_H__
#define __GETOPT_W32_H__

/* LICENSE: see LICENSE.getopt at the top-level directory. */

extern int optind;
extern char *optarg;
extern int opterr;

int getopt (int argc, char *argv[], const char *optstring);

#endif
