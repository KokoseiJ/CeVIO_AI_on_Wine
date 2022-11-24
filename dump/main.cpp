#ifdef __MINGW32__
#include <initguid.h>
#else
#pragma comment(lib, "wbemuuid.lib")
#endif

#include <kcnf/kcnf.h>
#include <wbemcli.h>
#include <windows.h>
#include <stdio.h>

#define LEN(arr) ((int) (sizeof(arr) / sizeof(arr[0])))
#define QUERY_LEN 100
#define NAME_LEN 5
#define NAME_STR_LEN 30

IWbemServices *pSvc = NULL;

wchar_t queries[][QUERY_LEN] = {
	L"select Caption, CSDVersion, Version from Win32_OperatingSystem",
	L"select TotalVisibleMemorySize, FreePhysicalMemory from Win32_OperatingSystem",
	L"select Name, CurrentClockSpeed from Win32_Processor",
	L"select Caption, DriverVersion from Win32_VideoController",
	L"select Caption from Win32_SoundDevice",
	L"select Name, NetConnectionID, AdapterType, PhysicalAdapter, NetEnabled from Win32_NetworkAdapter"
};

wchar_t names[][NAME_LEN][NAME_STR_LEN] {
	{L"Caption", L"CSDVersion", L"Version"},
	{L"TotalVisibleMemorySize", L"FreePyhsicalMemory"},
	{L"Name", L"CurrentClockSpeed"},
	{L"Caption", L"DriverVersion"},
	{L"Caption"},
	{L"Name", L"NetConnectionID", L"AdapterType", L"PhysicalAdapter", L"NetEnabled"}
};


IWbemServices *initialize_wbem() {
	HRESULT hres;
	BSTR tempBstr;
	IWbemLocator *pLoc = NULL;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres)) {
		printf("[!] Failed to initialize COM: 0x%08x\n", hres);
		return NULL;
	}

	hres =  CoInitializeSecurity(
		NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL
	);
	if (FAILED(hres)) {
		printf("[!] Failed to initialize Security: x=0x%08x\n", hres);
		CoUninitialize();
		return NULL;
	}

	hres = CoCreateInstance(
		CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		(LPVOID *) &pLoc
	);
	if (FAILED(hres)) {
		printf("[!] Failed to create IWbemLocator: 0x%08x\n", hres);
		CoUninitialize();
		return NULL;
	}

	tempBstr = SysAllocString(L"ROOT\\CIMV2");
	hres = pLoc->ConnectServer(tempBstr, NULL, NULL, 0, 0, 0, 0, &pSvc);
	SysFreeString(tempBstr);
	
	if (FAILED(hres)) {
		printf("[!] Failed to connect to CIMV2 WMI namespace: 0x%08x\n", hres);
		pLoc->Release();
		CoUninitialize();
		return NULL;
	}

	hres = CoSetProxyBlanket(
		pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE
	);
	if (FAILED(hres)) {
		printf("[!] Failed to set Proxy Blanket: 0x%08x\n", hres);
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return NULL;
	}

	pLoc->Release();

	return pSvc;
}


void execute_query(wchar_t *query, wchar_t keys[NAME_LEN][NAME_STR_LEN]) {
	static const BSTR wqlStr = SysAllocString(L"WQL");
	static const long flags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;

	int i = 0;
	ULONG uReturn = 0, returns;
	HRESULT hres;
	BSTR queryStr, keyStr;
	IEnumWbemClassObject *pEnum = NULL;
	IWbemClassObject *pWbemObj = NULL;
	VARIANT vt;
	CIMTYPE cimType;

	queryStr = SysAllocString(query);

	printf("\n\n[*] Query: %S\n", queryStr);

	hres = pSvc->ExecQuery(wqlStr, queryStr, flags, NULL, &pEnum);

	returns = 0;
	while (1) {
		hres = pEnum->Next(WBEM_INFINITE, 1, &pWbemObj, &uReturn);
		if (uReturn < 1) break;
		returns += uReturn;
		printf("\n[*] %lu\n", returns);

		for (i=0; i<NAME_LEN; i++) {
			if (*keys[i] == L'\0') break;
			keyStr = SysAllocString(keys[i]);

			VariantInit(&vt);
			hres = pWbemObj->Get(keyStr, 0, &vt, &cimType, NULL);

			if (FAILED(hres)) {
				printf("[!] Failed to retrieve key %S!\n", keyStr);
				continue;
			}

			printf("[*] cimType: %d, vt: %d\n", cimType, vt.vt);
			if (vt.vt == VT_BSTR || cimType == CIM_STRING) {
				printf("[*] %S : %S\n", keyStr, vt.bstrVal);
			} else {
				printf("[*] %S : not string. Type: %d\n", keyStr, cimType);
			}

			VariantClear(&vt);
		}
		pWbemObj->Release();
	}
	pEnum->Release();

	return;
}


int main() {
	int i = 0;

	initialize_wbem();
	if (pSvc == NULL) {
		printf("[!] Failed to initialize Wbem client.\n");
		return 1;
	}

	for (i=0; i<LEN(queries); i++)
		execute_query(queries[i], names[i]);

	return 0;
}
