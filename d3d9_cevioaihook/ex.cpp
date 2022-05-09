#include "ex.h"

HANDLE consoneHandle;

Linkedlist *llist;

BOOL(*orig_EnumServicesStatusW)(SC_HANDLE, DWORD, DWORD, LPENUM_SERVICE_STATUSW, DWORD, LPDWORD, LPDWORD, LPDWORD) = NULL;
ULONG(*orig_GetAdaptersAddresses)(ULONG, ULONG, PVOID, PIP_ADAPTER_ADDRESSES, PULONG) = NULL;
DWORD(*orig_GetPerAdapterInfo)(ULONG, PIP_PER_ADAPTER_INFO, PULONG) = NULL;


DWORD print(const char* stringBuf) {
	DWORD stringBufLen;
	DWORD written_chars;

	stringBufLen = static_cast<DWORD>(strlen(stringBuf));

	WriteConsole(
		consoneHandle,
		stringBuf,
		stringBufLen,
		&written_chars,
		NULL
	);

	return written_chars;
}

HANDLE setup_console() {
	char stringBuf[256];

	AllocConsole();
	SetConsoleTitle("CeVIO AI INJECTION DEBUG CONSOLE");
	consoneHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	sprintf_s(
		stringBuf,
		128 * sizeof(char),
		"[*] (setup_console) \t| Console Attached. StdHandle: %d\n", (int)consoneHandle
	);
	print(stringBuf);

	return stdout;
}


char* splitstr(char** str, char delim) {
	char* rtnstr, * splitptr;
	rtnstr = *str;
	splitptr = strchr(*str, delim);
	if (splitptr == NULL) {
		*str = NULL;
		print("[*] (splitstr) \t| This should've never happened. what have you done??\n");
	}
	else {
		*splitptr = '\0';
		*str = splitptr + 1;
	}
	return rtnstr;
}

long int flen(FILE* pFile) {
	long pos, end;

	pos = ftell(pFile);
	fseek(pFile, 0, SEEK_END);
	end = ftell(pFile);
	fseek(pFile, pos, SEEK_SET);

	return end - pos;
}


PIP_ADAPTER_ADDRESSES format_adapters() {
	uint8_t len, i;
	char stringBuf[256];
	PIP_ADAPTER_ADDRESSES rtnval;
	PIP_ADAPTER_ADDRESSES* ptr;

	len = *((uint8_t*)(llist_get(llist, "adapterlen")->value));

	sprintf_s(stringBuf, 256 * sizeof(char), "[*] (format_adapters) \t| Length: %u\n", len);
	print(stringBuf);

	ptr = &rtnval;

	for (i = 0; i < len; i++) {
		sprintf_s(stringBuf, 256 * sizeof(char), "adapter%u", i);
		*ptr = (PIP_ADAPTER_ADDRESSES)(llist_get(llist, stringBuf)->value);
		sprintf_s(stringBuf, 256 * sizeof(char), "adapter%uname", i);
		(*ptr)->AdapterName = (char*)(llist_get(llist, stringBuf)->value);
		sprintf_s(stringBuf, 256 * sizeof(char), "adapter%udesc", i);
		(*ptr)->Description = (wchar_t*)(llist_get(llist, stringBuf)->value);

		sprintf_s(stringBuf, 256 * sizeof(char), "adapter%uipv4", i);
		if (llist_get(llist, stringBuf) != NULL)
			(*ptr)->Dhcpv4Server.lpSockaddr = (LPSOCKADDR)(llist_get(llist, stringBuf)->value);

		sprintf_s(stringBuf, 256 * sizeof(char), "adapter%uipv6", i);
		if (llist_get(llist, stringBuf) != NULL)
			(*ptr)->Dhcpv6Server.lpSockaddr = (LPSOCKADDR)(llist_get(llist, stringBuf)->value);

		(*ptr)->FirstUnicastAddress = NULL;
		(*ptr)->FirstAnycastAddress = NULL;
		(*ptr)->FirstMulticastAddress = NULL;
		(*ptr)->FirstDnsServerAddress = NULL;
		(*ptr)->DnsSuffix = NULL;
		(*ptr)->FriendlyName = NULL;
		(*ptr)->FirstPrefix = NULL;
		(*ptr)->FirstWinsServerAddress = NULL;
		(*ptr)->FirstGatewayAddress = NULL;

		ptr = &((*ptr)->Next);
	}

	return rtnval;
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
	char stringBuf[256];
	_ENUM_SERVICE_STATUSW service;
	SERVICE_STATUS status;

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
		stringBuf,
		256 * sizeof(char),
		"[*] (hook_EnumServicesStatusW) \t| Called. Type: %ld, State: %ld, BufSize: %ld\n",
		dwServiceType, dwServiceState, cbBufSize
	);
	print(stringBuf);

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
			// if (lpServices[i].lpServiceName != "Winmgmt") continue;
			service = lpServices[i];
			status = service.ServiceStatus;
			sprintf_s(
				stringBuf,
				256 * sizeof(char),
				"[*] (hook_EnumServicesStatusW) \t| %ls, Display: %ls, Service | Type: %lx, State: %lx, Accepted: %lx, Exitcode: %lx, SpecExit: %lx, Check: %lx, Waithint: %lx\n",
				service.lpServiceName, service.lpDisplayName, status.dwServiceType, status.dwCurrentState, status.dwControlsAccepted, status.dwWin32ExitCode, status.dwServiceSpecificExitCode, status.dwCheckPoint, status.dwWaitHint
			);
			print(stringBuf);
		}

	}

	return true;
}

ULONG hook_GetAdaptersAddresses(
	ULONG Family,                           // IN
	ULONG Flags,                            // IN
	PVOID Reserved,                         // IN
	PIP_ADAPTER_ADDRESSES AdapterAddresses, // IN/OUT
	PULONG SizePointer                      // IN/OUT
) {
	unsigned long origBufSize = *SizePointer, rtnval = 0;
	char stringBuf[256];
	PIP_ADAPTER_ADDRESSES buffer;
	Linkedlist* entry;

	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (hook_GetAdaptersAddresses) \t| Called. Family: %lx, Flag: %lx, BufSize: %ld\n",
		Family, Flags, origBufSize
	);
	print(stringBuf);

	if (Flags == (GAA_FLAG_INCLUDE_WINS_INFO | GAA_FLAG_INCLUDE_GATEWAYS)) {
		entry = llist_get(llist, "adapter0");
		if (entry != NULL) {
			print("[*] (hook_GetAdaptersAddresses) \t| Dump found, overriding call value\n");

			buffer = format_adapters();

			if (origBufSize >= buffer->Length) {
				memcpy(AdapterAddresses, buffer, buffer->Length);
				rtnval = ERROR_SUCCESS;
			}
			else {
				*SizePointer = buffer->Length;
				sprintf_s(
					stringBuf,
					256 * sizeof(char),
					"[*] (hook_GetAdaptersAddresses) \t| sizeptr: %lu, size: %lu\n",
					*SizePointer, buffer->Length
				);
				print(stringBuf);
				rtnval = ERROR_BUFFER_OVERFLOW;
			}
		}
	}
	else
		rtnval = orig_GetAdaptersAddresses(Family, Flags, Reserved, AdapterAddresses, SizePointer);

	print("[*] (hook_GetAdaptersAddresses) \t| Reached the end\n");
	return rtnval;
}

DWORD hook_GetPerAdapterInfo(
	ULONG IfIndex,
	PIP_PER_ADAPTER_INFO pPerAdapterInfo,
	PULONG pOutBufLen
) {
	char stringBuf[256];

	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (hook_GetPerAdapterInfo) \t| index: %lu, size: %lu\n",
		IfIndex, *pOutBufLen
	);
	print(stringBuf);

	if (*pOutBufLen < sizeof(IP_PER_ADAPTER_INFO)) {
		*pOutBufLen = sizeof(IP_PER_ADAPTER_INFO);
		return ERROR_BUFFER_OVERFLOW;
	}

	*pPerAdapterInfo = {
		1,
		1,
		NULL,
		NULL
	};

	return ERROR_SUCCESS;
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
		print("[*] (create_hook) \t| Failed to enable EnumServicesStatusW.\n");
		exit(1);
	}
	print("[*] (create_hook) \t| Hooked EnumServicesStatusW.\n");

	if (MH_CreateHook(
		&GetAdaptersAddresses, &hook_GetAdaptersAddresses,
		reinterpret_cast<LPVOID*>(&orig_GetAdaptersAddresses)
	) != MH_OK) {
		print("[*] (create_hook) \t| Failed to hook GetAdaptersAddresses.\n");
		exit(1);
	}
	if (MH_EnableHook(&GetAdaptersAddresses) != MH_OK) {
		print("[*] (create_hook) \t| Failed to enable GetAdaptersAddresses.\n");
		exit(1);
	}
	print("[*] (create_hook) \t| Hooked GetAdaptersAddresses.\n");

	if (MH_CreateHook(
		&GetPerAdapterInfo, &hook_GetPerAdapterInfo,
		reinterpret_cast<LPVOID*>(&orig_GetPerAdapterInfo)
	) != MH_OK) {
		print("[*] (create_hook) \t| Failed to hook GetPerAdapterInfo.\n");
		exit(1);
	}
	if (MH_EnableHook(&GetPerAdapterInfo) != MH_OK) {
		print("[*] (create_hook) \t| Failed to enable GetPerAdapterInfo.\n");
		exit(1);
	}
	print("[*] (create_hook) \t| Hooked GetPerAdapterInfo.\n");

	return;
}

Linkedlist* load_ini(const char* filename) {
	FILE* pFile;
	char stringBuf[256], * str, * strptr, * name, * b64in;
	void* b64out;
	long int size;
	size_t b64inlen, b64outlen;

	fopen_s(&pFile, filename, "r");
	if (pFile == NULL) {
		sprintf_s(
			stringBuf,
			256 * sizeof(char),
			"[*] (load_ini) \t| Failed to load [%s], all the values will default to zero.\n",
			filename
		);
		print(stringBuf);
		return NULL;
	}
	print("[*] (load_ini) \t| Loaded File.\n");

	size = flen(pFile);
	str = strptr = (char*)calloc(size + sizeof(char), 1);
	if (str == NULL) {
		print("[*] (load_ini) \t| Failed to allocate string buffer.\n");
		return NULL;
	}
	fread(str, size, sizeof(char), pFile);
	fclose(pFile);

	while (strptr != NULL && *strptr != NULL) {
		name = splitstr(&strptr, '\n');
		b64in = splitstr(&strptr, '\n');
		b64inlen = strlen(b64in);
		b64outlen = b64declen(b64in);
		b64out = calloc(b64outlen, 1);


		b64decode(b64in, b64inlen, b64out, b64outlen);

		llist = llist_put(llist, name, b64out, b64outlen);

		sprintf_s(
			stringBuf,
			256 * sizeof(char),
			"[*] (load_ini) \t| Loaded Key: %s, size: %zu.\n",
			name, b64outlen
		);
		print(stringBuf);
	}

	print("[*] (load_ini) \t| List successfully loaded!\n");

	return NULL;
}


void ex_start() {
	setup_console();
	initialize_hook();
	load_ini("dump.cfg");
	create_hook();
}
