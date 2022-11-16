#ifndef INCL_DEBUG_H
#define INCL_DEBUG_H

#include <windows.h>
#include <string.h>
#include <stdio.h>

#define GREEN "\x1b[32m"
#define RED "\x1b[31m"
#define RESET "\x1b[0m"

DWORD debug_print(char *);
DWORD debug_printf(char *, ...);
DWORD debug_info(char *, char *, ...);
DWORD debug_error(char *, char *, ...);

int init_console();

int info_message(char*);
int error_message(char *);

#endif

