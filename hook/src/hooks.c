#include "hooks.h"


BOOL (*orig_EnumServicesStatusW)(SC_HANDLE, DWORD, DWORD, LPENUM_SERVICE_STATUSW, DWORD, LPDWORD, LPDWORD, LPDWORD) = NULL;


BOOL hook_EnumServicesStatusW(
	SC_HANDLE		hSCManager,		// IN
	DWORD 			dwServiceType,		// IN
	DWORD 			dwServiceState,		// IN
	LPENUM_SERVICE_STATUSW 	lpServices,		// OUT/OPT
	DWORD 			cbBufSize,		// IN
	LPDWORD 		pcbBytesNeeded,		// OUT
	LPDWORD 		lpServicesReturned,	// OUT
	LPDWORD 		lpResumeHandle 		// IN/OUT/OPT
) {
	/*
	EnumServicesStatus(intPtr, serviceType, 3, (IntPtr)0, 0, out num2, out num3, ref num);
	intPtr2 = Marshal.AllocHGlobal((IntPtr)num2);
	EnumServicesStatus(intPtr, serviceType, 3, intPtr2, num2, out num2, out num3, ref num);
	*/
	
	BOOL rtnval;
	size_t struct_size;

	debug_printf(
		"[*] (hook_EnumServicesStatusW) \t| Called. Type: 0x%08x, State: 0x%01x, cbBufSize: %d\n",
		dwServiceType, dwServiceState, cbBufSize
	);

	if (dwServiceType == SERVICE_WIN32 && dwServiceState == SERVICE_STATE_ALL) {
		debug_print("[*] (hook_EnumServicesStatusW) \t| Generating ENUM_SERVICE_STATUS...\n");
		
		struct_size = sizeof(ENUM_SERVICE_STATUSW);
		
		if (cbBufSize == 0) {
			debug_printf(
				"[*] (hook_EnumServicesStatusW) \t| cbBufSize == 0, bytesNeeded = %lld\n", struct_size
			);
			*pcbBytesNeeded = struct_size;
			return TRUE;
		} else if (cbBufSize < struct_size) {
			debug_printf(
				"[!] (hook_EnumServicesStatusW) \t| cbBufSize == %d < %lld! returning FALSE\n",
				cbBufSize, struct_size
			);
			*pcbBytesNeeded = struct_size;
			return FALSE;
		}
		
		lpServices[0] = (ENUM_SERVICE_STATUSW) {
			(WCHAR *) L"Winmgmt",	        // lpServiceName
			(WCHAR *) L"Windows Management Instrumentation", // lpDisplayName
			(SERVICE_STATUS) {	        // ServiceStatus
				0x30,
				SERVICE_RUNNING,
				0x7,
				0,
				0,
				0,
				0
			}
		};
            
                *lpServicesReturned = 1;
		
		return TRUE;
	}
	
	if (orig_EnumServicesStatusW == NULL) {
		debug_print("[!] (hook_EnumServicesStatusW) \t| Trampoline unavailable! returning FALSE\n");
		return FALSE;
	}

	rtnval = orig_EnumServicesStatusW(
			hSCManager,
			dwServiceType,
			dwServiceState,
			lpServices,
			cbBufSize,
			pcbBytesNeeded,
			lpServicesReturned,
			lpResumeHandle
		);

	return rtnval;
}


int init_hooks() {
	if (MH_Initialize() != MH_OK) {
		error_message("Failed to initialize MinHook!");
		return 1;
	}

	if (MH_CreateHook(&EnumServicesStatusW, &hook_EnumServicesStatusW, &orig_EnumServicesStatusW) != MH_OK) {
		error_message("Failed to create EnumServicesStatusW hook!");
		return 1;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		error_message("Failed to enable hooks!");
		return 1;
	}
	
	return 0;
}

