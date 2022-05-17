#pragma once
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "Wbemuuid.lib")

#ifndef INCL_EX_H
#define INCL_EX_H

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <synchapi.h>
#include <WbemCli.h>
#include <new.h>
#include "MinHook.h"
#include "llist.h"
#include "base64.h"

typedef struct comenumtype {
	CIMTYPE type;
	void* data;
} COMEnumType;

typedef struct wbemclasstype {
	const wchar_t* relpath;
	Linkedlist* com_llist;
} WbemClassType;

void ex_start();

#endif
