#ifndef INCL_DEBUG_H
#define INCL_DEBUG_H

#include <windows.h>
#include <string.h>
#include <stdio.h>


DWORD debug_print(char *);
DWORD debug_printf(char *, ...);
int init_console();

int info_message(char*);
int error_message(char *);

#endif

