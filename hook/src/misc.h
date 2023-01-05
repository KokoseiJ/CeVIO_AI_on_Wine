#ifndef INCL_MISC_H
#define INCL_MISC_H

#include "debug.h"
#include <windows.h>

const char *wine_get_version();
int wine_get_host_version(const char **sysname, const char **release);
int is_linux();

#endif

