#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static const char * progname;

noreturn void die(const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%s: ", getprogname());
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
    va_end(ap);
    exit(1);
}

void setprogname(const char * name)
{
    progname = name;
}

const char * getprogname()
{
    return progname;
}
