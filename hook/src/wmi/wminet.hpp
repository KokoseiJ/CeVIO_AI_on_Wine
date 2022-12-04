#ifndef INCL_WMINET_HPP
#define INCL_WMINET_HPP

#ifdef __MINGW32__
#include <initguid.h>
#else
#pragma comment(lib, "wbemuuid.lib")
#endif

#include "com/wbem.hpp"
extern "C" {
    #include "../debug.h"
}
#include <MinHook.h>
#include <wbemcli.h>
#include <windows.h>
#include <new>

extern "C" int init_wminet();

#endif
