// dllmain.cpp : Defines the entry point for the DLL application.

#include "ex.h"

#pragma comment(linker, "/export:ImmActivateLayout=\"C:\\Windows\\System32\\imm32.ImmActivateLayout\"")
#pragma comment(linker, "/export:ImmAssociateContext=\"C:\\Windows\\System32\\imm32.ImmAssociateContext\"")
#pragma comment(linker, "/export:ImmAssociateContextEx=\"C:\\Windows\\System32\\imm32.ImmAssociateContextEx\"")
#pragma comment(linker, "/export:ImmConfigureIMEA=\"C:\\Windows\\System32\\imm32.ImmConfigureIMEA\"")
#pragma comment(linker, "/export:ImmConfigureIMEW=\"C:\\Windows\\System32\\imm32.ImmConfigureIMEW\"")
#pragma comment(linker, "/export:ImmCreateContext=\"C:\\Windows\\System32\\imm32.ImmCreateContext\"")
#pragma comment(linker, "/export:ImmCreateIMCC=\"C:\\Windows\\System32\\imm32.ImmCreateIMCC\"")
#pragma comment(linker, "/export:ImmCreateSoftKeyboard=\"C:\\Windows\\System32\\imm32.ImmCreateSoftKeyboard\"")
#pragma comment(linker, "/export:ImmDestroyContext=\"C:\\Windows\\System32\\imm32.ImmDestroyContext\"")
#pragma comment(linker, "/export:ImmDestroyIMCC=\"C:\\Windows\\System32\\imm32.ImmDestroyIMCC\"")
#pragma comment(linker, "/export:ImmDestroySoftKeyboard=\"C:\\Windows\\System32\\imm32.ImmDestroySoftKeyboard\"")
#pragma comment(linker, "/export:ImmDisableIME=\"C:\\Windows\\System32\\imm32.ImmDisableIME\"")
#pragma comment(linker, "/export:ImmDisableIme=\"C:\\Windows\\System32\\imm32.ImmDisableIme\"")
#pragma comment(linker, "/export:ImmDisableLegacyIME=\"C:\\Windows\\System32\\imm32.ImmDisableLegacyIME\"")
#pragma comment(linker, "/export:ImmDisableTextFrameService=\"C:\\Windows\\System32\\imm32.ImmDisableTextFrameService\"")
#pragma comment(linker, "/export:ImmEnumInputContext=\"C:\\Windows\\System32\\imm32.ImmEnumInputContext\"")
#pragma comment(linker, "/export:ImmEnumRegisterWordA=\"C:\\Windows\\System32\\imm32.ImmEnumRegisterWordA\"")
#pragma comment(linker, "/export:ImmEnumRegisterWordW=\"C:\\Windows\\System32\\imm32.ImmEnumRegisterWordW\"")
#pragma comment(linker, "/export:ImmEscapeA=\"C:\\Windows\\System32\\imm32.ImmEscapeA\"")
#pragma comment(linker, "/export:ImmEscapeW=\"C:\\Windows\\System32\\imm32.ImmEscapeW\"")
#pragma comment(linker, "/export:ImmFreeLayout=\"C:\\Windows\\System32\\imm32.ImmFreeLayout\"")
#pragma comment(linker, "/export:ImmGenerateMessage=\"C:\\Windows\\System32\\imm32.ImmGenerateMessage\"")
#pragma comment(linker, "/export:ImmGetCandidateListA=\"C:\\Windows\\System32\\imm32.ImmGetCandidateListA\"")
#pragma comment(linker, "/export:ImmGetCandidateListCountA=\"C:\\Windows\\System32\\imm32.ImmGetCandidateListCountA\"")
#pragma comment(linker, "/export:ImmGetCandidateListCountW=\"C:\\Windows\\System32\\imm32.ImmGetCandidateListCountW\"")
#pragma comment(linker, "/export:ImmGetCandidateListW=\"C:\\Windows\\System32\\imm32.ImmGetCandidateListW\"")
#pragma comment(linker, "/export:ImmGetCandidateWindow=\"C:\\Windows\\System32\\imm32.ImmGetCandidateWindow\"")
#pragma comment(linker, "/export:ImmGetCompositionFontA=\"C:\\Windows\\System32\\imm32.ImmGetCompositionFontA\"")
#pragma comment(linker, "/export:ImmGetCompositionFontW=\"C:\\Windows\\System32\\imm32.ImmGetCompositionFontW\"")
#pragma comment(linker, "/export:ImmGetCompositionString=\"C:\\Windows\\System32\\imm32.ImmGetCompositionString\"")
#pragma comment(linker, "/export:ImmGetCompositionStringA=\"C:\\Windows\\System32\\imm32.ImmGetCompositionStringA\"")
#pragma comment(linker, "/export:ImmGetCompositionStringW=\"C:\\Windows\\System32\\imm32.ImmGetCompositionStringW\"")
#pragma comment(linker, "/export:ImmGetCompositionWindow=\"C:\\Windows\\System32\\imm32.ImmGetCompositionWindow\"")
#pragma comment(linker, "/export:ImmGetContext=\"C:\\Windows\\System32\\imm32.ImmGetContext\"")
#pragma comment(linker, "/export:ImmGetConversionListA=\"C:\\Windows\\System32\\imm32.ImmGetConversionListA\"")
#pragma comment(linker, "/export:ImmGetConversionListW=\"C:\\Windows\\System32\\imm32.ImmGetConversionListW\"")
#pragma comment(linker, "/export:ImmGetConversionStatus=\"C:\\Windows\\System32\\imm32.ImmGetConversionStatus\"")
#pragma comment(linker, "/export:ImmGetDefaultIMEWnd=\"C:\\Windows\\System32\\imm32.ImmGetDefaultIMEWnd\"")
#pragma comment(linker, "/export:ImmGetDescriptionA=\"C:\\Windows\\System32\\imm32.ImmGetDescriptionA\"")
#pragma comment(linker, "/export:ImmGetDescriptionW=\"C:\\Windows\\System32\\imm32.ImmGetDescriptionW\"")
#pragma comment(linker, "/export:ImmGetGuideLineA=\"C:\\Windows\\System32\\imm32.ImmGetGuideLineA\"")
#pragma comment(linker, "/export:ImmGetGuideLineW=\"C:\\Windows\\System32\\imm32.ImmGetGuideLineW\"")
#pragma comment(linker, "/export:ImmGetHotKey=\"C:\\Windows\\System32\\imm32.ImmGetHotKey\"")
#pragma comment(linker, "/export:ImmGetIMCCLockCount=\"C:\\Windows\\System32\\imm32.ImmGetIMCCLockCount\"")
#pragma comment(linker, "/export:ImmGetIMCCSize=\"C:\\Windows\\System32\\imm32.ImmGetIMCCSize\"")
#pragma comment(linker, "/export:ImmGetIMCLockCount=\"C:\\Windows\\System32\\imm32.ImmGetIMCLockCount\"")
#pragma comment(linker, "/export:ImmGetIMEFileNameA=\"C:\\Windows\\System32\\imm32.ImmGetIMEFileNameA\"")
#pragma comment(linker, "/export:ImmGetIMEFileNameW=\"C:\\Windows\\System32\\imm32.ImmGetIMEFileNameW\"")
#pragma comment(linker, "/export:ImmGetImeInfoEx=\"C:\\Windows\\System32\\imm32.ImmGetImeInfoEx\"")
#pragma comment(linker, "/export:ImmGetImeMenuItemsA=\"C:\\Windows\\System32\\imm32.ImmGetImeMenuItemsA\"")
#pragma comment(linker, "/export:ImmGetImeMenuItemsW=\"C:\\Windows\\System32\\imm32.ImmGetImeMenuItemsW\"")
#pragma comment(linker, "/export:ImmGetOpenStatus=\"C:\\Windows\\System32\\imm32.ImmGetOpenStatus\"")
#pragma comment(linker, "/export:ImmGetProperty=\"C:\\Windows\\System32\\imm32.ImmGetProperty\"")
#pragma comment(linker, "/export:ImmGetRegisterWordStyleA=\"C:\\Windows\\System32\\imm32.ImmGetRegisterWordStyleA\"")
#pragma comment(linker, "/export:ImmGetRegisterWordStyleW=\"C:\\Windows\\System32\\imm32.ImmGetRegisterWordStyleW\"")
#pragma comment(linker, "/export:ImmGetStatusWindowPos=\"C:\\Windows\\System32\\imm32.ImmGetStatusWindowPos\"")
#pragma comment(linker, "/export:ImmGetVirtualKey=\"C:\\Windows\\System32\\imm32.ImmGetVirtualKey\"")
#pragma comment(linker, "/export:ImmIMPGetIMEA=\"C:\\Windows\\System32\\imm32.ImmIMPGetIMEA\"")
#pragma comment(linker, "/export:ImmIMPGetIMEW=\"C:\\Windows\\System32\\imm32.ImmIMPGetIMEW\"")
#pragma comment(linker, "/export:ImmIMPQueryIMEA=\"C:\\Windows\\System32\\imm32.ImmIMPQueryIMEA\"")
#pragma comment(linker, "/export:ImmIMPQueryIMEW=\"C:\\Windows\\System32\\imm32.ImmIMPQueryIMEW\"")
#pragma comment(linker, "/export:ImmIMPSetIMEA=\"C:\\Windows\\System32\\imm32.ImmIMPSetIMEA\"")
#pragma comment(linker, "/export:ImmIMPSetIMEW=\"C:\\Windows\\System32\\imm32.ImmIMPSetIMEW\"")
#pragma comment(linker, "/export:ImmInstallIMEA=\"C:\\Windows\\System32\\imm32.ImmInstallIMEA\"")
#pragma comment(linker, "/export:ImmInstallIMEW=\"C:\\Windows\\System32\\imm32.ImmInstallIMEW\"")
#pragma comment(linker, "/export:ImmIsIME=\"C:\\Windows\\System32\\imm32.ImmIsIME\"")
#pragma comment(linker, "/export:ImmIsUIMessageA=\"C:\\Windows\\System32\\imm32.ImmIsUIMessageA\"")
#pragma comment(linker, "/export:ImmIsUIMessageW=\"C:\\Windows\\System32\\imm32.ImmIsUIMessageW\"")
#pragma comment(linker, "/export:ImmLoadIME=\"C:\\Windows\\System32\\imm32.ImmLoadIME\"")
#pragma comment(linker, "/export:ImmLoadLayout=\"C:\\Windows\\System32\\imm32.ImmLoadLayout\"")
#pragma comment(linker, "/export:ImmLockClientImc=\"C:\\Windows\\System32\\imm32.ImmLockClientImc\"")
#pragma comment(linker, "/export:ImmLockIMC=\"C:\\Windows\\System32\\imm32.ImmLockIMC\"")
#pragma comment(linker, "/export:ImmLockIMCC=\"C:\\Windows\\System32\\imm32.ImmLockIMCC\"")
#pragma comment(linker, "/export:ImmLockImeDpi=\"C:\\Windows\\System32\\imm32.ImmLockImeDpi\"")
#pragma comment(linker, "/export:ImmNotifyIME=\"C:\\Windows\\System32\\imm32.ImmNotifyIME\"")
#pragma comment(linker, "/export:ImmPenAuxInput=\"C:\\Windows\\System32\\imm32.ImmPenAuxInput\"")
#pragma comment(linker, "/export:ImmProcessKey=\"C:\\Windows\\System32\\imm32.ImmProcessKey\"")
#pragma comment(linker, "/export:ImmPutImeMenuItemsIntoMappedFile=\"C:\\Windows\\System32\\imm32.ImmPutImeMenuItemsIntoMappedFile\"")
#pragma comment(linker, "/export:ImmReSizeIMCC=\"C:\\Windows\\System32\\imm32.ImmReSizeIMCC\"")
#pragma comment(linker, "/export:ImmRegisterClient=\"C:\\Windows\\System32\\imm32.ImmRegisterClient\"")
#pragma comment(linker, "/export:ImmRegisterWordA=\"C:\\Windows\\System32\\imm32.ImmRegisterWordA\"")
#pragma comment(linker, "/export:ImmRegisterWordW=\"C:\\Windows\\System32\\imm32.ImmRegisterWordW\"")
#pragma comment(linker, "/export:ImmReleaseContext=\"C:\\Windows\\System32\\imm32.ImmReleaseContext\"")
#pragma comment(linker, "/export:ImmRequestMessageA=\"C:\\Windows\\System32\\imm32.ImmRequestMessageA\"")
#pragma comment(linker, "/export:ImmRequestMessageW=\"C:\\Windows\\System32\\imm32.ImmRequestMessageW\"")
#pragma comment(linker, "/export:ImmSendIMEMessageExA=\"C:\\Windows\\System32\\imm32.ImmSendIMEMessageExA\"")
#pragma comment(linker, "/export:ImmSendIMEMessageExW=\"C:\\Windows\\System32\\imm32.ImmSendIMEMessageExW\"")
#pragma comment(linker, "/export:ImmSendMessageToActiveDefImeWndW=\"C:\\Windows\\System32\\imm32.ImmSendMessageToActiveDefImeWndW\"")
#pragma comment(linker, "/export:ImmSetActiveContext=\"C:\\Windows\\System32\\imm32.ImmSetActiveContext\"")
#pragma comment(linker, "/export:ImmSetActiveContextConsoleIME=\"C:\\Windows\\System32\\imm32.ImmSetActiveContextConsoleIME\"")
#pragma comment(linker, "/export:ImmSetCandidateWindow=\"C:\\Windows\\System32\\imm32.ImmSetCandidateWindow\"")
#pragma comment(linker, "/export:ImmSetCompositionFontA=\"C:\\Windows\\System32\\imm32.ImmSetCompositionFontA\"")
#pragma comment(linker, "/export:ImmSetCompositionFontW=\"C:\\Windows\\System32\\imm32.ImmSetCompositionFontW\"")
#pragma comment(linker, "/export:ImmSetCompositionStringA=\"C:\\Windows\\System32\\imm32.ImmSetCompositionStringA\"")
#pragma comment(linker, "/export:ImmSetCompositionStringW=\"C:\\Windows\\System32\\imm32.ImmSetCompositionStringW\"")
#pragma comment(linker, "/export:ImmSetCompositionWindow=\"C:\\Windows\\System32\\imm32.ImmSetCompositionWindow\"")
#pragma comment(linker, "/export:ImmSetConversionStatus=\"C:\\Windows\\System32\\imm32.ImmSetConversionStatus\"")
#pragma comment(linker, "/export:ImmSetOpenStatus=\"C:\\Windows\\System32\\imm32.ImmSetOpenStatus\"")
#pragma comment(linker, "/export:ImmSetStatusWindowPos=\"C:\\Windows\\System32\\imm32.ImmSetStatusWindowPos\"")
#pragma comment(linker, "/export:ImmShowSoftKeyboard=\"C:\\Windows\\System32\\imm32.ImmShowSoftKeyboard\"")
#pragma comment(linker, "/export:ImmSimulateHotKey=\"C:\\Windows\\System32\\imm32.ImmSimulateHotKey\"")
#pragma comment(linker, "/export:ImmSystemHandler=\"C:\\Windows\\System32\\imm32.ImmSystemHandler\"")
#pragma comment(linker, "/export:ImmTranslateMessage=\"C:\\Windows\\System32\\imm32.ImmTranslateMessage\"")
#pragma comment(linker, "/export:ImmUnlockClientImc=\"C:\\Windows\\System32\\imm32.ImmUnlockClientImc\"")
#pragma comment(linker, "/export:ImmUnlockIMC=\"C:\\Windows\\System32\\imm32.ImmUnlockIMC\"")
#pragma comment(linker, "/export:ImmUnlockIMCC=\"C:\\Windows\\System32\\imm32.ImmUnlockIMCC\"")
#pragma comment(linker, "/export:ImmUnlockImeDpi=\"C:\\Windows\\System32\\imm32.ImmUnlockImeDpi\"")
#pragma comment(linker, "/export:ImmUnregisterWordA=\"C:\\Windows\\System32\\imm32.ImmUnregisterWordA\"")
#pragma comment(linker, "/export:ImmUnregisterWordW=\"C:\\Windows\\System32\\imm32.ImmUnregisterWordW\"")
#pragma comment(linker, "/export:ImmWINNLSEnableIME=\"C:\\Windows\\System32\\imm32.ImmWINNLSEnableIME\"")
#pragma comment(linker, "/export:ImmWINNLSGetEnableStatus=\"C:\\Windows\\System32\\imm32.ImmWINNLSGetEnableStatus\"")
#pragma comment(linker, "/export:ImmWINNLSGetIMEHotkey=\"C:\\Windows\\System32\\imm32.ImmWINNLSGetIMEHotkey\"")
#pragma comment(linker, "/export:__wine_get_ui_window=\"C:\\Windows\\System32\\imm32.__wine_get_ui_window\"")
#pragma comment(linker, "/export:__wine_register_window=\"C:\\Windows\\System32\\imm32.__wine_register_window\"")
#pragma comment(linker, "/export:__wine_unregister_window=\"C:\\Windows\\System32\\imm32.__wine_unregister_window\"")

#pragma comment(linker, "/export:WldpAddDeveloperCertificateForDynamicCodeTrust=\"C:\\Windows\\System32\\Wldp.WldpAddDeveloperCertificateForDynamicCodeTrust\"")
#pragma comment(linker, "/export:WldpCheckDeviceEncryptionNotStarted=\"C:\\Windows\\System32\\Wldp.WldpCheckDeviceEncryptionNotStarted\"")
#pragma comment(linker, "/export:WldpCheckRetailConfiguration=\"C:\\Windows\\System32\\Wldp.WldpCheckRetailConfiguration\"")
#pragma comment(linker, "/export:WldpCheckSecurityWatermarkState=\"C:\\Windows\\System32\\Wldp.WldpCheckSecurityWatermarkState\"")
#pragma comment(linker, "/export:WldpCheckWcosDeviceEncryptionSecure=\"C:\\Windows\\System32\\Wldp.WldpCheckWcosDeviceEncryptionSecure\"")
#pragma comment(linker, "/export:WldpDisableDeveloperMode=\"C:\\Windows\\System32\\Wldp.WldpDisableDeveloperMode\"")
#pragma comment(linker, "/export:WldpEnableDeveloperMode=\"C:\\Windows\\System32\\Wldp.WldpEnableDeveloperMode\"")
#pragma comment(linker, "/export:WldpGetLockdownPolicy=\"C:\\Windows\\System32\\Wldp.WldpGetLockdownPolicy\"")
#pragma comment(linker, "/export:WldpIsAllowedEntryPoint=\"C:\\Windows\\System32\\Wldp.WldpIsAllowedEntryPoint\"")
#pragma comment(linker, "/export:WldpIsAppApprovedByPolicy=\"C:\\Windows\\System32\\Wldp.WldpIsAppApprovedByPolicy\"")
#pragma comment(linker, "/export:WldpIsClassInApprovedList=\"C:\\Windows\\System32\\Wldp.WldpIsClassInApprovedList\"")
#pragma comment(linker, "/export:WldpIsDebugAllowed=\"C:\\Windows\\System32\\Wldp.WldpIsDebugAllowed\"")
#pragma comment(linker, "/export:WldpIsDynamicCodePolicyEnabled=\"C:\\Windows\\System32\\Wldp.WldpIsDynamicCodePolicyEnabled\"")
#pragma comment(linker, "/export:WldpQueryDynamicCodeTrust=\"C:\\Windows\\System32\\Wldp.WldpQueryDynamicCodeTrust\"")
#pragma comment(linker, "/export:WldpQuerySecurityPolicy=\"C:\\Windows\\System32\\Wldp.WldpQuerySecurityPolicy\"")
#pragma comment(linker, "/export:WldpQueryWindowsLockdownMode=\"C:\\Windows\\System32\\Wldp.WldpQueryWindowsLockdownMode\"")
#pragma comment(linker, "/export:WldpQueryWindowsLockdownRestriction=\"C:\\Windows\\System32\\Wldp.WldpQueryWindowsLockdownRestriction\"")
#pragma comment(linker, "/export:WldpResetSecurityWatermarkState=\"C:\\Windows\\System32\\Wldp.WldpResetSecurityWatermarkState\"")
#pragma comment(linker, "/export:WldpSetDynamicCodeTrust=\"C:\\Windows\\System32\\Wldp.WldpSetDynamicCodeTrust\"")
#pragma comment(linker, "/export:WldpSetDynamicCodeTrust2=\"C:\\Windows\\System32\\Wldp.WldpSetDynamicCodeTrust2\"")
#pragma comment(linker, "/export:WldpSetWindowsLockdownRestriction=\"C:\\Windows\\System32\\Wldp.WldpSetWindowsLockdownRestriction\"")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ex_start();
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

