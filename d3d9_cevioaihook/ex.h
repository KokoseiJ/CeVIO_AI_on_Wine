#pragma once
#pragma comment(lib, "iphlpapi.lib")

#ifndef INCL_EX_H
#define INCL_EX_H

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include "MinHook.h"

void ex_start();

#endif
