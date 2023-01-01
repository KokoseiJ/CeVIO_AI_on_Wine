#ifndef __MINGW32__ // imm32 exports for MSVC, pragma linker comment is not usable with MinGW

#pragma comment(linker, "/export:ImmActivateLayout=\"C:\\windows\\system32\\imm32.ImmActivateLayout\"")
#pragma comment(linker, "/export:ImmAssociateContext=\"C:\\windows\\system32\\imm32.ImmAssociateContext\"")
#pragma comment(linker, "/export:ImmAssociateContextEx=\"C:\\windows\\system32\\imm32.ImmAssociateContextEx\"")
#pragma comment(linker, "/export:ImmConfigureIMEA=\"C:\\windows\\system32\\imm32.ImmConfigureIMEA\"")
#pragma comment(linker, "/export:ImmConfigureIMEW=\"C:\\windows\\system32\\imm32.ImmConfigureIMEW\"")
#pragma comment(linker, "/export:ImmCreateContext=\"C:\\windows\\system32\\imm32.ImmCreateContext\"")
#pragma comment(linker, "/export:ImmCreateIMCC=\"C:\\windows\\system32\\imm32.ImmCreateIMCC\"")
#pragma comment(linker, "/export:ImmCreateSoftKeyboard=\"C:\\windows\\system32\\imm32.ImmCreateSoftKeyboard\"")
#pragma comment(linker, "/export:ImmDestroyContext=\"C:\\windows\\system32\\imm32.ImmDestroyContext\"")
#pragma comment(linker, "/export:ImmDestroyIMCC=\"C:\\windows\\system32\\imm32.ImmDestroyIMCC\"")
#pragma comment(linker, "/export:ImmDestroySoftKeyboard=\"C:\\windows\\system32\\imm32.ImmDestroySoftKeyboard\"")
#pragma comment(linker, "/export:ImmDisableIME=\"C:\\windows\\system32\\imm32.ImmDisableIME\"")
#pragma comment(linker, "/export:ImmDisableIme=\"C:\\windows\\system32\\imm32.ImmDisableIme\"")
#pragma comment(linker, "/export:ImmDisableLegacyIME=\"C:\\windows\\system32\\imm32.ImmDisableLegacyIME\"")
#pragma comment(linker, "/export:ImmDisableTextFrameService=\"C:\\windows\\system32\\imm32.ImmDisableTextFrameService\"")
#pragma comment(linker, "/export:ImmEnumInputContext=\"C:\\windows\\system32\\imm32.ImmEnumInputContext\"")
#pragma comment(linker, "/export:ImmEnumRegisterWordA=\"C:\\windows\\system32\\imm32.ImmEnumRegisterWordA\"")
#pragma comment(linker, "/export:ImmEnumRegisterWordW=\"C:\\windows\\system32\\imm32.ImmEnumRegisterWordW\"")
#pragma comment(linker, "/export:ImmEscapeA=\"C:\\windows\\system32\\imm32.ImmEscapeA\"")
#pragma comment(linker, "/export:ImmEscapeW=\"C:\\windows\\system32\\imm32.ImmEscapeW\"")
#pragma comment(linker, "/export:ImmFreeLayout=\"C:\\windows\\system32\\imm32.ImmFreeLayout\"")
#pragma comment(linker, "/export:ImmGenerateMessage=\"C:\\windows\\system32\\imm32.ImmGenerateMessage\"")
#pragma comment(linker, "/export:ImmGetCandidateListA=\"C:\\windows\\system32\\imm32.ImmGetCandidateListA\"")
#pragma comment(linker, "/export:ImmGetCandidateListCountA=\"C:\\windows\\system32\\imm32.ImmGetCandidateListCountA\"")
#pragma comment(linker, "/export:ImmGetCandidateListCountW=\"C:\\windows\\system32\\imm32.ImmGetCandidateListCountW\"")
#pragma comment(linker, "/export:ImmGetCandidateListW=\"C:\\windows\\system32\\imm32.ImmGetCandidateListW\"")
#pragma comment(linker, "/export:ImmGetCandidateWindow=\"C:\\windows\\system32\\imm32.ImmGetCandidateWindow\"")
#pragma comment(linker, "/export:ImmGetCompositionFontA=\"C:\\windows\\system32\\imm32.ImmGetCompositionFontA\"")
#pragma comment(linker, "/export:ImmGetCompositionFontW=\"C:\\windows\\system32\\imm32.ImmGetCompositionFontW\"")
#pragma comment(linker, "/export:ImmGetCompositionString=\"C:\\windows\\system32\\imm32.ImmGetCompositionString\"")
#pragma comment(linker, "/export:ImmGetCompositionStringA=\"C:\\windows\\system32\\imm32.ImmGetCompositionStringA\"")
#pragma comment(linker, "/export:ImmGetCompositionStringW=\"C:\\windows\\system32\\imm32.ImmGetCompositionStringW\"")
#pragma comment(linker, "/export:ImmGetCompositionWindow=\"C:\\windows\\system32\\imm32.ImmGetCompositionWindow\"")
#pragma comment(linker, "/export:ImmGetContext=\"C:\\windows\\system32\\imm32.ImmGetContext\"")
#pragma comment(linker, "/export:ImmGetConversionListA=\"C:\\windows\\system32\\imm32.ImmGetConversionListA\"")
#pragma comment(linker, "/export:ImmGetConversionListW=\"C:\\windows\\system32\\imm32.ImmGetConversionListW\"")
#pragma comment(linker, "/export:ImmGetConversionStatus=\"C:\\windows\\system32\\imm32.ImmGetConversionStatus\"")
#pragma comment(linker, "/export:ImmGetDefaultIMEWnd=\"C:\\windows\\system32\\imm32.ImmGetDefaultIMEWnd\"")
#pragma comment(linker, "/export:ImmGetDescriptionA=\"C:\\windows\\system32\\imm32.ImmGetDescriptionA\"")
#pragma comment(linker, "/export:ImmGetDescriptionW=\"C:\\windows\\system32\\imm32.ImmGetDescriptionW\"")
#pragma comment(linker, "/export:ImmGetGuideLineA=\"C:\\windows\\system32\\imm32.ImmGetGuideLineA\"")
#pragma comment(linker, "/export:ImmGetGuideLineW=\"C:\\windows\\system32\\imm32.ImmGetGuideLineW\"")
#pragma comment(linker, "/export:ImmGetHotKey=\"C:\\windows\\system32\\imm32.ImmGetHotKey\"")
#pragma comment(linker, "/export:ImmGetIMCCLockCount=\"C:\\windows\\system32\\imm32.ImmGetIMCCLockCount\"")
#pragma comment(linker, "/export:ImmGetIMCCSize=\"C:\\windows\\system32\\imm32.ImmGetIMCCSize\"")
#pragma comment(linker, "/export:ImmGetIMCLockCount=\"C:\\windows\\system32\\imm32.ImmGetIMCLockCount\"")
#pragma comment(linker, "/export:ImmGetIMEFileNameA=\"C:\\windows\\system32\\imm32.ImmGetIMEFileNameA\"")
#pragma comment(linker, "/export:ImmGetIMEFileNameW=\"C:\\windows\\system32\\imm32.ImmGetIMEFileNameW\"")
#pragma comment(linker, "/export:ImmGetImeInfoEx=\"C:\\windows\\system32\\imm32.ImmGetImeInfoEx\"")
#pragma comment(linker, "/export:ImmGetImeMenuItemsA=\"C:\\windows\\system32\\imm32.ImmGetImeMenuItemsA\"")
#pragma comment(linker, "/export:ImmGetImeMenuItemsW=\"C:\\windows\\system32\\imm32.ImmGetImeMenuItemsW\"")
#pragma comment(linker, "/export:ImmGetOpenStatus=\"C:\\windows\\system32\\imm32.ImmGetOpenStatus\"")
#pragma comment(linker, "/export:ImmGetProperty=\"C:\\windows\\system32\\imm32.ImmGetProperty\"")
#pragma comment(linker, "/export:ImmGetRegisterWordStyleA=\"C:\\windows\\system32\\imm32.ImmGetRegisterWordStyleA\"")
#pragma comment(linker, "/export:ImmGetRegisterWordStyleW=\"C:\\windows\\system32\\imm32.ImmGetRegisterWordStyleW\"")
#pragma comment(linker, "/export:ImmGetStatusWindowPos=\"C:\\windows\\system32\\imm32.ImmGetStatusWindowPos\"")
#pragma comment(linker, "/export:ImmGetVirtualKey=\"C:\\windows\\system32\\imm32.ImmGetVirtualKey\"")
#pragma comment(linker, "/export:ImmIMPGetIMEA=\"C:\\windows\\system32\\imm32.ImmIMPGetIMEA\"")
#pragma comment(linker, "/export:ImmIMPGetIMEW=\"C:\\windows\\system32\\imm32.ImmIMPGetIMEW\"")
#pragma comment(linker, "/export:ImmIMPQueryIMEA=\"C:\\windows\\system32\\imm32.ImmIMPQueryIMEA\"")
#pragma comment(linker, "/export:ImmIMPQueryIMEW=\"C:\\windows\\system32\\imm32.ImmIMPQueryIMEW\"")
#pragma comment(linker, "/export:ImmIMPSetIMEA=\"C:\\windows\\system32\\imm32.ImmIMPSetIMEA\"")
#pragma comment(linker, "/export:ImmIMPSetIMEW=\"C:\\windows\\system32\\imm32.ImmIMPSetIMEW\"")
#pragma comment(linker, "/export:ImmInstallIMEA=\"C:\\windows\\system32\\imm32.ImmInstallIMEA\"")
#pragma comment(linker, "/export:ImmInstallIMEW=\"C:\\windows\\system32\\imm32.ImmInstallIMEW\"")
#pragma comment(linker, "/export:ImmIsIME=\"C:\\windows\\system32\\imm32.ImmIsIME\"")
#pragma comment(linker, "/export:ImmIsUIMessageA=\"C:\\windows\\system32\\imm32.ImmIsUIMessageA\"")
#pragma comment(linker, "/export:ImmIsUIMessageW=\"C:\\windows\\system32\\imm32.ImmIsUIMessageW\"")
#pragma comment(linker, "/export:ImmLoadIME=\"C:\\windows\\system32\\imm32.ImmLoadIME\"")
#pragma comment(linker, "/export:ImmLoadLayout=\"C:\\windows\\system32\\imm32.ImmLoadLayout\"")
#pragma comment(linker, "/export:ImmLockClientImc=\"C:\\windows\\system32\\imm32.ImmLockClientImc\"")
#pragma comment(linker, "/export:ImmLockIMC=\"C:\\windows\\system32\\imm32.ImmLockIMC\"")
#pragma comment(linker, "/export:ImmLockIMCC=\"C:\\windows\\system32\\imm32.ImmLockIMCC\"")
#pragma comment(linker, "/export:ImmLockImeDpi=\"C:\\windows\\system32\\imm32.ImmLockImeDpi\"")
#pragma comment(linker, "/export:ImmNotifyIME=\"C:\\windows\\system32\\imm32.ImmNotifyIME\"")
#pragma comment(linker, "/export:ImmPenAuxInput=\"C:\\windows\\system32\\imm32.ImmPenAuxInput\"")
#pragma comment(linker, "/export:ImmProcessKey=\"C:\\windows\\system32\\imm32.ImmProcessKey\"")
#pragma comment(linker, "/export:ImmPutImeMenuItemsIntoMappedFile=\"C:\\windows\\system32\\imm32.ImmPutImeMenuItemsIntoMappedFile\"")
#pragma comment(linker, "/export:ImmReSizeIMCC=\"C:\\windows\\system32\\imm32.ImmReSizeIMCC\"")
#pragma comment(linker, "/export:ImmRegisterClient=\"C:\\windows\\system32\\imm32.ImmRegisterClient\"")
#pragma comment(linker, "/export:ImmRegisterWordA=\"C:\\windows\\system32\\imm32.ImmRegisterWordA\"")
#pragma comment(linker, "/export:ImmRegisterWordW=\"C:\\windows\\system32\\imm32.ImmRegisterWordW\"")
#pragma comment(linker, "/export:ImmReleaseContext=\"C:\\windows\\system32\\imm32.ImmReleaseContext\"")
#pragma comment(linker, "/export:ImmRequestMessageA=\"C:\\windows\\system32\\imm32.ImmRequestMessageA\"")
#pragma comment(linker, "/export:ImmRequestMessageW=\"C:\\windows\\system32\\imm32.ImmRequestMessageW\"")
#pragma comment(linker, "/export:ImmSendIMEMessageExA=\"C:\\windows\\system32\\imm32.ImmSendIMEMessageExA\"")
#pragma comment(linker, "/export:ImmSendIMEMessageExW=\"C:\\windows\\system32\\imm32.ImmSendIMEMessageExW\"")
#pragma comment(linker, "/export:ImmSendMessageToActiveDefImeWndW=\"C:\\windows\\system32\\imm32.ImmSendMessageToActiveDefImeWndW\"")
#pragma comment(linker, "/export:ImmSetActiveContext=\"C:\\windows\\system32\\imm32.ImmSetActiveContext\"")
#pragma comment(linker, "/export:ImmSetActiveContextConsoleIME=\"C:\\windows\\system32\\imm32.ImmSetActiveContextConsoleIME\"")
#pragma comment(linker, "/export:ImmSetCandidateWindow=\"C:\\windows\\system32\\imm32.ImmSetCandidateWindow\"")
#pragma comment(linker, "/export:ImmSetCompositionFontA=\"C:\\windows\\system32\\imm32.ImmSetCompositionFontA\"")
#pragma comment(linker, "/export:ImmSetCompositionFontW=\"C:\\windows\\system32\\imm32.ImmSetCompositionFontW\"")
#pragma comment(linker, "/export:ImmSetCompositionStringA=\"C:\\windows\\system32\\imm32.ImmSetCompositionStringA\"")
#pragma comment(linker, "/export:ImmSetCompositionStringW=\"C:\\windows\\system32\\imm32.ImmSetCompositionStringW\"")
#pragma comment(linker, "/export:ImmSetCompositionWindow=\"C:\\windows\\system32\\imm32.ImmSetCompositionWindow\"")
#pragma comment(linker, "/export:ImmSetConversionStatus=\"C:\\windows\\system32\\imm32.ImmSetConversionStatus\"")
#pragma comment(linker, "/export:ImmSetOpenStatus=\"C:\\windows\\system32\\imm32.ImmSetOpenStatus\"")
#pragma comment(linker, "/export:ImmSetStatusWindowPos=\"C:\\windows\\system32\\imm32.ImmSetStatusWindowPos\"")
#pragma comment(linker, "/export:ImmShowSoftKeyboard=\"C:\\windows\\system32\\imm32.ImmShowSoftKeyboard\"")
#pragma comment(linker, "/export:ImmSimulateHotKey=\"C:\\windows\\system32\\imm32.ImmSimulateHotKey\"")
#pragma comment(linker, "/export:ImmSystemHandler=\"C:\\windows\\system32\\imm32.ImmSystemHandler\"")
#pragma comment(linker, "/export:ImmTranslateMessage=\"C:\\windows\\system32\\imm32.ImmTranslateMessage\"")
#pragma comment(linker, "/export:ImmUnlockClientImc=\"C:\\windows\\system32\\imm32.ImmUnlockClientImc\"")
#pragma comment(linker, "/export:ImmUnlockIMC=\"C:\\windows\\system32\\imm32.ImmUnlockIMC\"")
#pragma comment(linker, "/export:ImmUnlockIMCC=\"C:\\windows\\system32\\imm32.ImmUnlockIMCC\"")
#pragma comment(linker, "/export:ImmUnlockImeDpi=\"C:\\windows\\system32\\imm32.ImmUnlockImeDpi\"")
#pragma comment(linker, "/export:ImmUnregisterWordA=\"C:\\windows\\system32\\imm32.ImmUnregisterWordA\"")
#pragma comment(linker, "/export:ImmUnregisterWordW=\"C:\\windows\\system32\\imm32.ImmUnregisterWordW\"")
#pragma comment(linker, "/export:ImmWINNLSEnableIME=\"C:\\windows\\system32\\imm32.ImmWINNLSEnableIME\"")
#pragma comment(linker, "/export:ImmWINNLSGetEnableStatus=\"C:\\windows\\system32\\imm32.ImmWINNLSGetEnableStatus\"")
#pragma comment(linker, "/export:ImmWINNLSGetIMEHotkey=\"C:\\windows\\system32\\imm32.ImmWINNLSGetIMEHotkey\"")
#pragma comment(linker, "/export:__wine_get_ui_window=\"C:\\windows\\system32\\imm32.__wine_get_ui_window\"")
#pragma comment(linker, "/export:__wine_register_window=\"C:\\windows\\system32\\imm32.__wine_register_window\"")
#pragma comment(linker, "/export:__wine_unregister_window=\"C:\\windows\\system32\\imm32.__wine_unregister_window\"")

#endif // _MINGW32__

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

