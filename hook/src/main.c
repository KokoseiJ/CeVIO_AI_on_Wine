#include <MinHook.h>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            printf("[*] Hello, World!\n");
	    MessageBox(NULL, "Hello, World!", "TestBox", MB_OK);
    }
    return TRUE;
}

