// dllmain.cpp : Defines the entry point for the DLL application.

#pragma comment(linker, "/export:WldpAddDeveloperCertificateForDynamicCodeTrust=\"C:\\Windows\\System32\\wldp.WldpAddDeveloperCertificateForDynamicCodeTrust\"")
#pragma comment(linker, "/export:WldpCheckDeviceEncryptionNotStarted=\"C:\\Windows\\System32\\wldp.WldpCheckDeviceEncryptionNotStarted\"")
#pragma comment(linker, "/export:WldpCheckRetailConfiguration=\"C:\\Windows\\System32\\wldp.WldpCheckRetailConfiguration\"")
#pragma comment(linker, "/export:WldpCheckSecurityWatermarkState=\"C:\\Windows\\System32\\wldp.WldpCheckSecurityWatermarkState\"")
#pragma comment(linker, "/export:WldpCheckWcosDeviceEncryptionSecure=\"C:\\Windows\\System32\\wldp.WldpCheckWcosDeviceEncryptionSecure\"")
#pragma comment(linker, "/export:WldpDisableDeveloperMode=\"C:\\Windows\\System32\\wldp.WldpDisableDeveloperMode\"")
#pragma comment(linker, "/export:WldpEnableDeveloperMode=\"C:\\Windows\\System32\\wldp.WldpEnableDeveloperMode\"")
#pragma comment(linker, "/export:WldpGetLockdownPolicy=\"C:\\Windows\\System32\\wldp.WldpGetLockdownPolicy\"")
#pragma comment(linker, "/export:WldpIsAllowedEntryPoint=\"C:\\Windows\\System32\\wldp.WldpIsAllowedEntryPoint\"")
#pragma comment(linker, "/export:WldpIsAppApprovedByPolicy=\"C:\\Windows\\System32\\wldp.WldpIsAppApprovedByPolicy\"")
#pragma comment(linker, "/export:WldpIsClassInApprovedList=\"C:\\Windows\\System32\\wldp.WldpIsClassInApprovedList\"")
#pragma comment(linker, "/export:WldpIsDebugAllowed=\"C:\\Windows\\System32\\wldp.WldpIsDebugAllowed\"")
#pragma comment(linker, "/export:WldpIsDynamicCodePolicyEnabled=\"C:\\Windows\\System32\\wldp.WldpIsDynamicCodePolicyEnabled\"")
#pragma comment(linker, "/export:WldpQueryDynamicCodeTrust=\"C:\\Windows\\System32\\wldp.WldpQueryDynamicCodeTrust\"")
#pragma comment(linker, "/export:WldpQuerySecurityPolicy=\"C:\\Windows\\System32\\wldp.WldpQuerySecurityPolicy\"")
#pragma comment(linker, "/export:WldpQueryWindowsLockdownMode=\"C:\\Windows\\System32\\wldp.WldpQueryWindowsLockdownMode\"")
#pragma comment(linker, "/export:WldpQueryWindowsLockdownRestriction=\"C:\\Windows\\System32\\wldp.WldpQueryWindowsLockdownRestriction\"")
#pragma comment(linker, "/export:WldpResetSecurityWatermarkState=\"C:\\Windows\\System32\\wldp.WldpResetSecurityWatermarkState\"")
#pragma comment(linker, "/export:WldpSetDynamicCodeTrust=\"C:\\Windows\\System32\\wldp.WldpSetDynamicCodeTrust\"")
#pragma comment(linker, "/export:WldpSetDynamicCodeTrust2=\"C:\\Windows\\System32\\wldp.WldpSetDynamicCodeTrust2\"")
#pragma comment(linker, "/export:WldpSetWindowsLockdownRestriction=\"C:\\Windows\\System32\\wldp.WldpSetWindowsLockdownRestriction\"")

#include <Windows.h>
#include "ex.h"

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

