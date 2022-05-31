#include "ex.h"

Linkedlist* llist, * network_llist = NULL, * volume_llist = NULL, ** sound_llist_list = NULL;
size_t sound_llist_len = 0;

BOOL(*orig_EnumServicesStatusW)(SC_HANDLE, DWORD, DWORD, LPENUM_SERVICE_STATUSW, DWORD, LPDWORD, LPDWORD, LPDWORD) = NULL;
ULONG(*orig_GetAdaptersAddresses)(ULONG, ULONG, PVOID, PIP_ADAPTER_ADDRESSES, PULONG) = NULL;
DWORD(*orig_GetPerAdapterInfo)(ULONG, PIP_PER_ADAPTER_INFO, PULONG) = NULL;
HRESULT(*orig_ExecQueryWmi)(BSTR, BSTR, long, IWbemContext*, IEnumWbemClassObject**, DWORD, DWORD, IWbemServices*, BSTR, BSTR, BSTR);
HRESULT(*orig_CloneEnumWbemClassObject)(IEnumWbemClassObject**, DWORD, DWORD, IEnumWbemClassObject*, BSTR, BSTR, BSTR);


// =============== Hook-specific subcomponents ===============
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

void init_wmi_network_llist() {
	COMEnumType* enumdata;
	void* buffer;
	
	if (network_llist != NULL) return;

	buffer = calloc(17, sizeof(wchar_t));
	if (buffer == NULL) {
		print("[*] (init_wmi_network_llist) \t| Failed to allocate buffer.\n");
		exit(1);
	}
	wcscpy_s((wchar_t*)buffer, 17, L"IA BEST VOCALOID\0");
	enumdata = (COMEnumType*)calloc(1, sizeof(enumdata));
	if (enumdata == NULL) {
		print("[*] (init_wmi_network_llist) \t| Failed to allocate buffer.\n");
		exit(1);
	}
	*enumdata = {
		CIM_STRING,
		buffer
	};
	network_llist = llist_put(network_llist, "NetConnectionID", enumdata, sizeof(enumdata));
	network_llist = llist_put(network_llist, "Name", enumdata, sizeof(enumdata));
	network_llist = llist_put(network_llist, "AdapterType", enumdata, sizeof(enumdata));

	buffer = calloc(1, sizeof(bool));
	if (buffer == NULL) {
		print("[*] (init_wmi_network_llist) \t| Failed to allocate buffer.\n");
		exit(1);
	}
	*((bool*)buffer) = true;
	enumdata = (COMEnumType*)calloc(1, sizeof(enumdata));
	if (enumdata == NULL) {
		print("[*] (init_wmi_network_llist) \t| Failed to allocate buffer.\n");
		exit(1);
	}
	*enumdata = {
		CIM_BOOLEAN,
		buffer
	};

	network_llist = llist_put(network_llist, "NetEnabled", enumdata, sizeof(enumdata));
	network_llist = llist_put(network_llist, "PhysicalAdapter", enumdata, sizeof(enumdata));

	return;
}

void init_wmi_volume_llist() {
	COMEnumType* enumdata;
	char* volumeserial;
	wchar_t* wcsvolserial;
	size_t i;

	if (volume_llist != NULL) return;

	volumeserial = (char*)(llist_get(llist, "hdPrimarySerial")->value);

	wcsvolserial = (wchar_t*)calloc(strlen(volumeserial) + 1, sizeof(wchar_t));

	mbstowcs_s(&i, wcsvolserial, strlen(volumeserial) + 1, volumeserial, (strlen(volumeserial) + 1) * sizeof(wchar_t));

	enumdata = (COMEnumType*)calloc(1, sizeof(enumdata));
	if (enumdata == NULL) {
		print("[*] (init_wmi_network_llist) \t| Failed to allocate buffer.\n");
		exit(1);
	}
	*enumdata = {
		CIM_STRING,
		wcsvolserial
	};

	volume_llist = llist_put(volume_llist, "VolumeSerialNumber", enumdata, sizeof(enumdata));

	return;
}

void init_wmi_sound_llist_list() {
	uint8_t soundlen, i;
	char *strPtr, nameBuf[256];
	wchar_t* wcsPtr;
	Linkedlist* sound_llist = NULL;
	COMEnumType* enumdata;
	size_t _;

	if (sound_llist_list != NULL) return;

	soundlen = *(uint8_t*)(llist_get(llist, "soundlen")->value);

	sound_llist_list = (Linkedlist**)calloc(soundlen, sizeof(void*));
	if (sound_llist_list == NULL) {
		print("[*] (init_wmi_sound_wbemclass) \t| Failed to allocate buffer.\n");
		exit(1);
	}

	for (i = 0; i < soundlen; i++) {
		sprintf_s(nameBuf, 256 * sizeof(char), "sound%uid", i);
		strPtr = (char*)llist_get(llist, nameBuf)->value;

		wcsPtr = (wchar_t*)calloc(strlen(strPtr) + 1, sizeof(wchar_t));
		mbstowcs_s(&_, wcsPtr, strlen(strPtr) + 1, strPtr, (strlen(strPtr) + 1) * sizeof(wchar_t));

		enumdata = (COMEnumType*)calloc(1, sizeof(COMEnumType));
		if (enumdata == NULL) {
			print("[*] (init_wmi_sound_wbemclass) \t| Failed to allocate buffer.\n");
			exit(1);
		}
		*enumdata = {
			CIM_STRING,
			wcsPtr
		};
		sound_llist = llist_put(sound_llist, "DeviceID", enumdata, sizeof(enumdata));

		sprintf_s(nameBuf, 256 * sizeof(char), "sound%ucaption", i);
		strPtr = (char*)llist_get(llist, nameBuf)->value;

		wcsPtr = (wchar_t*)calloc(strlen(strPtr) + 1, sizeof(wchar_t));
		mbstowcs_s(&_, wcsPtr, strlen(strPtr) + 1, strPtr, (strlen(strPtr) + 1) * sizeof(wchar_t));

		enumdata = (COMEnumType*)calloc(1, sizeof(COMEnumType));
		if (enumdata == NULL) {
			print("[*] (init_wmi_sound_wbemclass) \t| Failed to allocate buffer.\n");
			exit(1);
		}
		*enumdata = {
			CIM_STRING,
			wcsPtr
		};
		sound_llist = llist_put(sound_llist, "Caption", enumdata, sizeof(enumdata));
		sound_llist_list[i] = sound_llist;
		sound_llist = NULL;
	}

	sound_llist_len = soundlen;

	return;
}


// =============== HOOKS ===============
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

HRESULT hook_ExecQueryWmi(
	BSTR strQueryLanguage,
	BSTR strQuery,
	long lFlags,
	IWbemContext* pCtx,
	IEnumWbemClassObject** ppEnum,
	DWORD authLevel,
	DWORD impLevel,
	IWbemServices* pCurrentNamespace,
	BSTR strUser,
	BSTR strPassword,
	BSTR strAuthority
) {
	char stringBuf[256];

	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (hook_ExecQueryWmi) \t| Query Called: `%S`.\n",
		strQuery
	);
	print(stringBuf);

	if (wcscmp(strQuery, L"select Name, NetConnectionID, AdapterType, PhysicalAdapter, NetEnabled from Win32_NetworkAdapter") == 0) {
		init_wmi_network_llist();
		*ppEnum = new HookEnumWbemClassObject(network_llist, L"Win32_NetworkAdapter.Name=\"CIM_NetworkAdapter\"\0");
		return S_OK;
	}
	else if (wcscmp(strQuery, L"select VolumeSerialNumber from Win32_LogicalDisk where DeviceID=\"C:\"") == 0) {
		init_wmi_volume_llist();
		*ppEnum = new HookEnumWbemClassObject(volume_llist, L"Win32_LogicalDisk.Name=\"CIM_LogicalDisk\"\0");
		return S_OK;
	}
	else if (wcscmp(strQuery, L"select DeviceID from Win32_SoundDevice") == 0 || wcscmp(strQuery, L"select Caption from Win32_SoundDevice") == 0) {
		init_wmi_sound_llist_list();
		*ppEnum = new HookEnumWbemClassObject(sound_llist_list, sound_llist_len, L"Win32_SoundDevice.Name=\"CIM_SoundDevice\"\0");
		return S_OK;
	}

	print("[*] (hook_ExecQueryWmi) \t| Non-network query, proxying the call...\n");
	return orig_ExecQueryWmi(strQueryLanguage, strQuery, lFlags, pCtx, ppEnum, authLevel, impLevel, pCurrentNamespace, strUser, strPassword, strAuthority);
	
	//return 0x80041000;
}

HRESULT hook_CloneEnumWbemClassObject(
	IEnumWbemClassObject** ppEnum,
	DWORD authLevel,
	DWORD impLevel,
	IEnumWbemClassObject* pCurrentEnumWbemClassObject,
	BSTR strUser,
	BSTR strPassword,
	BSTR strAuthority
) {
	print("[*] (hook_CloneEnumWbemClassObject) \t| Called.\n");

	pCurrentEnumWbemClassObject->Clone(ppEnum);
	return S_OK;
}


// =============== Initialization codes ===============
void initialize_hook() {
	if (MH_Initialize() != MH_OK) {
		print("[*] (initialize_hook) \t| Minhook initialize failed.\n");
		exit(1);
	}

	print("[*] (initialize_hook) \t| Minhook initialized.\n");
	return;
}

void create_hook() {
	HMODULE wminetHandle;
	FARPROC ptr_ExecQueryWmi, ptr_CloneEnumWbemClassObject;

	wminetHandle = LoadLibraryA("C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319\\WMINet_Utils.dll");
	if (wminetHandle == NULL) {
		print("[*] (create_hook) \t| Failed to load WMINet_Utils.dll.\n");
		exit(1);
	}
	ptr_ExecQueryWmi = GetProcAddress(wminetHandle, "ExecQueryWmi");
	if (ptr_ExecQueryWmi == NULL) {
		print("[*] (create_hook) \t| Failed to load ExecQueryWmi.\n");
		exit(1);
	}

	ptr_CloneEnumWbemClassObject = GetProcAddress(wminetHandle, "CloneEnumWbemClassObject");
	if (ptr_ExecQueryWmi == NULL) {
		print("[*] (create_hook) \t| Failed to load CloneEnumWbemClassObject\n");
		exit(1);
	}

	//CloseHandle(wminetHandle);

	print("[*] (create_hook) \t| Loaded ExecQueryWmi address.\n");
	print("[*] (create_hook) \t| Loaded CloneEnumWbemClassObject address.\n");

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

	if (MH_CreateHook(
		ptr_ExecQueryWmi, &hook_ExecQueryWmi,
		reinterpret_cast<LPVOID*>(&orig_ExecQueryWmi)
	) != MH_OK) {
		print("[*] (create_hook) \t| Failed to hook ExecQueryWmi.\n");
		exit(1);
	}
	if (MH_EnableHook(ptr_ExecQueryWmi) != MH_OK) {
		print("[*] (create_hook) \t| Failed to enable ExecQueryWmi.\n");
		exit(1);
	}
	print("[*] (create_hook) \t| Hooked ExecQueryWmi.\n");

	if (MH_CreateHook(
		ptr_CloneEnumWbemClassObject, &hook_CloneEnumWbemClassObject,
		reinterpret_cast<LPVOID*>(&orig_CloneEnumWbemClassObject)
	) != MH_OK) {
		print("[*] (create_hook) \t| Failed to hook CloneEnumWbemClassObject.\n");
		exit(1);
	}
	if (MH_EnableHook(ptr_CloneEnumWbemClassObject) != MH_OK) {
		print("[*] (create_hook) \t| Failed to enable CloneEnumWbemClassObject.\n");
		exit(1);
	}
	print("[*] (create_hook) \t| Hooked CloneEnumWbemClassObject.\n");

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

	// TODO: Allocate key names to new strings and unallocate the file string
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
	load_ini("dump.cfg");
	initialize_hook();
	create_hook();
}
