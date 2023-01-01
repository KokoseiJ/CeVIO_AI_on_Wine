#include "debug.h"
#include "misc.h"
#include "hooks.h"
#include <MinHook.h>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>


int init_all() {
	if (init_console()) {
		error_message("Failed to initialize console!\nIt shouldn't interfere with the functionalty, but expect debug messages to not show up.");
	}

	if (init_hooks()) {
		error_message("Failed to apply hooks!");
		return 1;
	}
	
	info_message("CeVIO AI on Wine Successfully loaded!\nCopyright (C) 2022 Wonjun Jung (KokoseiJ)\n\nUse at your own risk, and have fun :D");
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	const char *wine_version, *wine_host;
	if (fdwReason == DLL_PROCESS_ATTACH) {
		wine_version = wine_get_version();
		// wine_host = wine_get_host_version();
		if (wine_version == NULL) {
			error_message("This doesn't seem to be running on WINE!\nI refuse to run on non-UNIX system :P");
			return TRUE;
		}
		
		if (init_all()) return FALSE;
		debug_print("[*] Hello, World!\n");
		// debug_printf("[*] Wine Version: %s\n    Host version: %s\n", wine_version, wine_host);
	}

	return TRUE;
}

