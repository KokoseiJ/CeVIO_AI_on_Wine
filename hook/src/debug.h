#ifndef INCL_DEBUG_H
#define INCL_DEBUG_H

#include <windows.h>
#include <string.h>
#include <stdio.h>

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

DWORD debug_print(const char *);
DWORD debug_printf(const char *, ...);
DWORD debug_info(const char *, const char *, ...);
DWORD debug_error(const char *, const char *, ...);

int init_console();

int info_message(const char*);
int error_message(const char *);

#endif

