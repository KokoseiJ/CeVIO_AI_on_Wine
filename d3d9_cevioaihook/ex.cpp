#include "ex.h"

HANDLE console_stdout;

BOOL(*orig_EnumServicesStatusW)(SC_HANDLE, DWORD, DWORD, LPENUM_SERVICE_STATUSW, DWORD, LPDWORD, LPDWORD, LPDWORD) = NULL;

DWORD print(const char* printstr) {
	DWORD printstr_length;
	DWORD written_chars;

	printstr_length = static_cast<DWORD>(strlen(printstr));

	WriteConsole(
		console_stdout,
		printstr,
		printstr_length,
		&written_chars,
		NULL
	);

	return written_chars;
}

HANDLE setup_console() {
	char* printarr;
	printarr = (char*)calloc(128, sizeof(char));
	if (printarr == NULL) return NULL;

	AllocConsole();
	SetConsoleTitle("CeVIO AI INJECTION DEBUG CONSOLE");
	console_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

	sprintf_s(
		printarr,
		128 * sizeof(char),
		"[*] (setup_console) \t| Console Attached. StdHandle: %d\n", (int)console_stdout
	);
	print(printarr);

	free(printarr);

	return stdout;
}

BOOL hook_EnumServicesStatusW(
	SC_HANDLE hSCManager,              // IN
	DWORD dwServiceType,               // IN
	DWORD dwServiceState,              // IN
	LPENUM_SERVICE_STATUSW lpServices, // OUT/OPT
	DWORD cbBufSize,                   // IN
	LPDWORD pcbBytesNeeded,            // OUT
	LPDWORD lpServicesReturned,        // OUT
	LPDWORD lpResumeHandle             // IN/OUT/OPT
) {
	unsigned int i;
	char* printarr;
	_ENUM_SERVICE_STATUSW service;
	SERVICE_STATUS status;

	printarr = (char*)calloc(256, sizeof(char));
	if (printarr == NULL) return false;

	/*
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
	*/

	sprintf_s(
		printarr,
		256 * sizeof(char),
		"[*] (hook_EnumServicesStatusW) \t| Called. Type: %ld, State: %ld, BufSize: %ld\n",
		dwServiceType, dwServiceState, cbBufSize
	);
	print(printarr);

	*pcbBytesNeeded = sizeof(ENUM_SERVICE_STATUSW);
	*lpServicesReturned = 1;

	if (cbBufSize != 0) {
		lpServices[0] = {
			(WCHAR *)L"Winmgmt",
			(WCHAR *)L"Windows Management Instrumentation",
			{
				0x30, // Apparently not included in MSDN Docs!
				SERVICE_RUNNING,
				0x7,
				0,
				0,	
				0,
				0
			}
		};

		for (i = 0; i < *lpServicesReturned; i++) {
			if (wcscmp(lpServices[i].lpServiceName, L"Winmgmt") != 0) continue;
			service = lpServices[i];
			status = service.ServiceStatus;
			sprintf_s(
				printarr,
				256 * sizeof(char),
				"[*] (hook_EnumServicesStatusW) \t| Winmgmt, Display: %ls, Service | Type: %lx, State: %lx, Accepted: %lx, Exitcode: %lx, SpecExit: %lx, Check: %lx, Waithint: %lx",
				service.lpDisplayName, status.dwServiceType, status.dwCurrentState, status.dwControlsAccepted, status.dwWin32ExitCode, status.dwServiceSpecificExitCode, status.dwCheckPoint, status.dwWaitHint
			);
			print(printarr);
		}

	}

	free(printarr);

	return true;
}

void initialize_hook() {
	if (MH_Initialize() != MH_OK) {
		print("[*] (initialize_hook) \t| Minhook initialize failed.\n");
		exit(1);
	}

	print("[*] (initialize_hook) \t| Minhook initialized.\n");
	return;
}

void create_hook() {
	if (MH_CreateHook(
		&EnumServicesStatusW, &hook_EnumServicesStatusW,
		reinterpret_cast<LPVOID*>(&orig_EnumServicesStatusW)
	) != MH_OK) {
		print("[*] (create_hook) \t| Failed to hook EnumServicesStatusW.\n");
		exit(1);
	}
	if (MH_EnableHook(&EnumServicesStatusW) != MH_OK) {
		print("[*](create_hook) \t| Failed to enable EnumServicesStatusW.\n");
		exit(1);
	}

	print("[*] (initialize_hook) \t| Hooked EnumServicesStatusW.\n");
	return;
}


void ex_start() {
	setup_console();
	initialize_hook();
	create_hook();
}
