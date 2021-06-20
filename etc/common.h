#pragma once

#include <stdnoreturn.h>

noreturn void die(const char * fmt, ...);
void usage();

void setprogname(const char * name);
const char * getprogname();
