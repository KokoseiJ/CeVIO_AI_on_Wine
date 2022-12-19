#ifndef INCL_COM_WBEM_CLIENT_HPP
#define INCL_COM_WBEM_CLIENT_HPP

#ifdef __MINGW32__
#include <initguid.h>
#else
#pragma comment(lib, "wbemuuid.lib")
#endif

extern "C" {
#include "../../debug.h"
}
#include <wbemcli.h>
#include <windows.h>


int init_wbemsvc();
IEnumWbemClassObject *get_pEnum(BSTR query);

#endif
