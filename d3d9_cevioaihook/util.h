#pragma once
#ifndef INCL_UTIL_H
#define INCL_UTIL_H

#include <stdio.h>
#include <Windows.h>

DWORD print(const char* stringBuf);

HANDLE setup_console();


char* splitstr(char** str, char delim);

long int flen(FILE* pFile);

#endif
