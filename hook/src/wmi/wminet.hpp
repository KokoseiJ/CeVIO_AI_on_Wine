#ifndef INCL_WMI_WMINET_HPP
#define INCL_WMI_WMINET_HPP

#ifdef __MINGW32__
#include <initguid.h>
#else
#pragma comment(lib, "wbemuuid.lib")
#endif

#include "com/wbem.hpp"
#include "com/wbem_proxy.hpp"
#include "com/wbem_client.hpp"
extern "C" {
    #include "../debug.h"
}
#include <MinHook.h>
#include <wbemcli.h>
#include <windows.h>
#include <new>

extern "C" int init_wminet();

#endif
