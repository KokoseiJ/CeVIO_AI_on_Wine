#include "ex.h"

HANDLE consoneHandle;

Linkedlist *llist;

BOOL(*orig_EnumServicesStatusW)(SC_HANDLE, DWORD, DWORD, LPENUM_SERVICE_STATUSW, DWORD, LPDWORD, LPDWORD, LPDWORD) = NULL;
ULONG(*orig_GetAdaptersAddresses)(ULONG, ULONG, PVOID, PIP_ADAPTER_ADDRESSES, PULONG) = NULL;


struct address_entry_copy_params
{
	IP_ADAPTER_ADDRESSES* src, * dst;
	char* ptr;
	void* next;
	ULONG cur_offset;
};

static void address_lists_iterate(IP_ADAPTER_ADDRESSES* aa, void (*fn)(void* entry, ULONG offset, void* ctxt), void* ctxt)
{
	IP_ADAPTER_UNICAST_ADDRESS* uni;
	IP_ADAPTER_DNS_SERVER_ADDRESS* dns;
	IP_ADAPTER_GATEWAY_ADDRESS* gw;
	IP_ADAPTER_PREFIX* prefix;
	void* next;

	for (uni = aa->FirstUnicastAddress; uni; uni = (PIP_ADAPTER_UNICAST_ADDRESS)next)
	{
		next = uni->Next;
		fn(uni, FIELD_OFFSET(IP_ADAPTER_ADDRESSES, FirstUnicastAddress), ctxt);
	}

	for (dns = aa->FirstDnsServerAddress; dns; dns = (PIP_ADAPTER_DNS_SERVER_ADDRESS)next)
	{
		next = dns->Next;
		fn(dns, FIELD_OFFSET(IP_ADAPTER_ADDRESSES, FirstDnsServerAddress), ctxt);
	}

	for (gw = aa->FirstGatewayAddress; gw; gw = (PIP_ADAPTER_GATEWAY_ADDRESS)next)
	{
		next = gw->Next;
		fn(gw, FIELD_OFFSET(IP_ADAPTER_ADDRESSES, FirstGatewayAddress), ctxt);
	}

	for (prefix = aa->FirstPrefix; prefix; prefix = (PIP_ADAPTER_PREFIX)next)
	{
		next = prefix->Next;
		fn(prefix, FIELD_OFFSET(IP_ADAPTER_ADDRESSES, FirstPrefix), ctxt);
	}
}

static void address_entry_copy(void* ptr, ULONG offset, void* ctxt)
{
	struct address_entry_copy_params* params = (struct address_entry_copy_params*)ctxt;
	IP_ADAPTER_DNS_SERVER_ADDRESS* src_addr = (PIP_ADAPTER_DNS_SERVER_ADDRESS)ptr; /* all list types are super-sets of this type */
	IP_ADAPTER_DNS_SERVER_ADDRESS* dst_addr = (IP_ADAPTER_DNS_SERVER_ADDRESS*)params->ptr;
	ULONG align = sizeof(ULONGLONG) - 1;

	memcpy(dst_addr, src_addr, src_addr->Length);
	params->ptr += src_addr->Length;
	dst_addr->Address.lpSockaddr = (SOCKADDR*)params->ptr;
	memcpy(dst_addr->Address.lpSockaddr, src_addr->Address.lpSockaddr, src_addr->Address.iSockaddrLength);
	params->ptr += (src_addr->Address.iSockaddrLength + align) & ~align;

	if (params->cur_offset != offset) /* new list */
	{
		params->next = (BYTE*)params->dst + offset;
		params->cur_offset = offset;
	}
	*(IP_ADAPTER_DNS_SERVER_ADDRESS**)params->next = dst_addr;
	params->next = &dst_addr->Next;
}

static void adapters_addresses_copy(IP_ADAPTER_ADDRESSES* dst, IP_ADAPTER_ADDRESSES* src)
{
	char* ptr;
	DWORD len;
	UINT_PTR align = sizeof(ULONGLONG) - 1;
	struct address_entry_copy_params params;

	while (src)
	{
		ptr = (char*)(dst + 1);
		*dst = *src;
		dst->AdapterName = ptr;
		len = strlen(src->AdapterName) + 1;
		memcpy(dst->AdapterName, src->AdapterName, len);
		ptr += (len + 1) & ~1;
		dst->Description = (WCHAR*)ptr;
		len = (wcslen(src->Description) + 1) * sizeof(WCHAR);
		memcpy(dst->Description, src->Description, len);
		ptr += len;
		dst->DnsSuffix = (WCHAR*)ptr;
		len = (wcslen(src->DnsSuffix) + 1) * sizeof(WCHAR);
		memcpy(dst->DnsSuffix, src->DnsSuffix, len);
		ptr += len;
		if (src->FriendlyName)
		{
			dst->FriendlyName = (WCHAR*)ptr;
			len = (wcslen(src->FriendlyName) + 1) * sizeof(WCHAR);
			memcpy(dst->FriendlyName, src->FriendlyName, len);
			ptr += len;
		}
		ptr = (char*)(((UINT_PTR)ptr + align) & ~align);

		params.src = src;
		params.dst = dst;
		params.ptr = ptr;
		params.next = NULL;
		params.cur_offset = ~0u;
		address_lists_iterate(src, address_entry_copy, &params);
		ptr = params.ptr;

		if (src->Next)
		{
			dst->Next = (IP_ADAPTER_ADDRESSES*)ptr;
			dst = dst->Next;
		}
		src = src->Next;
	}
}


DWORD print(const char* printstr) {
	DWORD printstr_length;
	DWORD written_chars;

	printstr_length = static_cast<DWORD>(strlen(printstr));

	WriteConsole(
		consoneHandle,
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
	consoneHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	sprintf_s(
		printarr,
		128 * sizeof(char),
		"[*] (setup_console) \t| Console Attached. StdHandle: %d\n", (int)consoneHandle
	);
	print(printarr);

	free(printarr);

	return stdout;
}


char* splitstr(char** str, char delim) {
	char* rtnstr, * splitptr;
	rtnstr = *str;
	splitptr = strchr(*str, delim);
	if (splitptr == NULL) {
		*str = NULL;
		print("[*] (splitstr) \t\t| This should've never happened. what have you done??\n");
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
			// if (lpServices[i].lpServiceName != "Winmgmt") continue;
			service = lpServices[i];
			status = service.ServiceStatus;
			sprintf_s(
				printarr,
				256 * sizeof(char),
				"[*] (hook_EnumServicesStatusW) \t| %ls, Display: %ls, Service | Type: %lx, State: %lx, Accepted: %lx, Exitcode: %lx, SpecExit: %lx, Check: %lx, Waithint: %lx\n",
				service.lpServiceName, service.lpDisplayName, status.dwServiceType, status.dwCurrentState, status.dwControlsAccepted, status.dwWin32ExitCode, status.dwServiceSpecificExitCode, status.dwCheckPoint, status.dwWaitHint
			);
			print(printarr);
		}

	}

	free(printarr);

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
	char* printarr;
	Linkedlist* entry;

	printarr = (char*)calloc(256, sizeof(char));
	if (printarr == NULL) return 0;

	sprintf_s(
		printarr,
		256 * sizeof(char),
		"[*] (hook_GetAdaptersAddresses) \t| Called. Family: %lx, Flag: %lx, BufSize: %ld\n",
		Family, Flags, origBufSize
	);
	print(printarr);

	if (Flags == (GAA_FLAG_INCLUDE_WINS_INFO | GAA_FLAG_INCLUDE_GATEWAYS)) {
		entry = llist_get(llist, "GetAdaptersAddresses");
		if (entry != NULL) {
			print("[*] (hook_GetAdaptersAddresses) \t| Dump found, overriding call value\n");
			if (origBufSize >= entry->size) {
				adapters_addresses_copy(AdapterAddresses, (PIP_ADAPTER_ADDRESSES)entry->value);
				rtnval = ERROR_SUCCESS;
			}
			else {
				*SizePointer = entry->size;
				rtnval = ERROR_BUFFER_OVERFLOW;
			}
		}
	}
	else
		rtnval = orig_GetAdaptersAddresses(Family, Flags, Reserved, AdapterAddresses, SizePointer);

	free(printarr);
	return rtnval;
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

	return;
}

Linkedlist* load_ini(const char* filename) {
	FILE* pFile;
	char* printarr, * str, * strptr, * name, * b64in;
	void* b64out;
	long int size;
	size_t b64inlen, b64outlen;

	printarr = (char*)calloc(256, sizeof(char));
	if (printarr == NULL) return 0;

	fopen_s(&pFile, filename, "r");
	if (pFile == NULL) {
		sprintf_s(
			printarr,
			256 * sizeof(char),
			"[*] (load_ini) \t| Failed to load [%s], all the values will default to zero.\n",
			filename
		);
		print(printarr);
		return NULL;
	}
	print("[*] (load_ini) \t\t| Loaded File.\n");

	size = flen(pFile);
	str = strptr = (char*)calloc(size + sizeof(char), 1);
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
			printarr,
			256 * sizeof(char),
			"[*] (load_ini) \t\t| Loaded Key: %s, size: %zu.\n",
			name, b64outlen
		);
		print(printarr);
	}

	print("[*] (load_ini) \t\t| List successfully loaded!\n");

	free(printarr);

	return NULL;
}


void ex_start() {
	setup_console();
	initialize_hook();
	load_ini("dump.cfg");
	create_hook();
}
