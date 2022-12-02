#ifndef INCL_WMINET_HPP
#define INCL_WMINET_HPP

//#include "wminet_c.h"
#include "com/wbem.hpp"
extern "C" {
    #include "../debug.h"
}
#include <MinHook.h>
#include <wbemcli.h>
#include <windows.h>
#include <new>

#pragma comment(lib, "wbemuuid.lib")

extern "C" int init_wminet();

#endif

