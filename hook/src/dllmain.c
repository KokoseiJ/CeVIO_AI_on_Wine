#include "misc.h"
#include <MinHook.h>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            printf("[*] Hello, World!\n");
	    MessageBox(NULL, wine_get_version(), "TestBox", MB_OK);
    }
    return TRUE;
}

