#pragma once
#pragma comment(lib, "iphlpapi.lib")

#ifndef INCL_EX_H
#define INCL_EX_H

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <synchapi.h>
#include <WbemCli.h>
#include "MinHook.h"
#include "llist.h"
#include "base64.h"

void ex_start();

#endif
