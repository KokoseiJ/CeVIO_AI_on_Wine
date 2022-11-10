#include "debug.h"
#include "misc.h"
#include "hooks.h"
#include <MinHook.h>
#include <stdio.h>
#include <windows.h>
#include <winuser.h>


void init_all() {
	if (init_console()) {
		error_message("Failed to initialize console!\nIt shouldn't interfere with the functionalty, but expect debug messages to not show up.");
	}

	if (init_hooks()) {
		error_message("Failed to apply hooks!");
	}
	
	info_message("CeVIO AI on Wine Successfully loaded!\nCopyright (C) 2022 Wonjun Jung (KokoseiJ)\n\nUse at your own risk, and have fun :D");
	return;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		if (wine_get_version() == NULL) {
			error_message("This doesn't seem to be running on WINE!\nI refuse to run on non-UNIX system :P");
			return TRUE;
		}
		
		debug_print("[*] Hello, World!\n");
		init_all();
	}

	return TRUE;
}

