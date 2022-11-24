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
#define QUERY_LEN 128
#define NAME_LEN 6
#define NAME_STR_LEN 30

IWbemServices *pSvc = NULL;

wchar_t queries[][NAME_LEN][NAME_STR_LEN] {
	{L"Win32_OperatingSystem", L"Caption", L"CSDVersion", L"Version"},
	{L"Win32_OperatingSystem", L"TotalVisibleMemorySize", L"FreePhysicalMemory"},
	{L"Win32_Processor", L"Name", L"CurrentClockSpeed"},
	{L"Win32_VideoController", L"Caption", L"DriverVersion"},
	{L"Win32_SoundDevice", L"Caption"},
	{L"Win32_NetworkAdapter", L"Name", L"NetConnectionID", L"AdapterType", L"PhysicalAdapter", L"NetEnabled"}
};


IWbemServices *initialize_wbem() {
	HRESULT hres;
	BSTR tempBstr;
	IWbemLocator *pLoc = NULL;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres)) {
		printf("[!] Failed to initialize COM: 0x%08lx\n", hres);
		return NULL;
	}

	hres =  CoInitializeSecurity(
		NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL
	);
	if (FAILED(hres)) {
		printf("[!] Failed to initialize Security: 0x%08lx\n", hres);
		CoUninitialize();
		return NULL;
	}

	hres = CoCreateInstance(
		CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
		(LPVOID *) &pLoc
	);
	if (FAILED(hres)) {
		printf("[!] Failed to create IWbemLocator: 0x%08lx\n", hres);
		CoUninitialize();
		return NULL;
	}

	tempBstr = SysAllocString(L"ROOT\\CIMV2");
	hres = pLoc->ConnectServer(tempBstr, NULL, NULL, 0, 0, 0, 0, &pSvc);
	SysFreeString(tempBstr);
	
	if (FAILED(hres)) {
		printf("[!] Failed to connect to CIMV2 WMI namespace: 0x%08lx\n", hres);
		pLoc->Release();
		CoUninitialize();
		return NULL;
	}

	hres = CoSetProxyBlanket(
		pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE
	);
	if (FAILED(hres)) {
		printf("[!] Failed to set Proxy Blanket: 0x%08lx\n", hres);
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return NULL;
	}

	pLoc->Release();

	return pSvc;
}

void iter_keys(IEnumWbemClassObject *, int);
void execute_query(int idx) {
	static const BSTR wqlStr = SysAllocString(L"WQL");
	static const long flags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;
	int i;
	wchar_t queryStrBuilder[QUERY_LEN] = L"select ";
	BSTR queryStr;
	IEnumWbemClassObject *pEnum = NULL;

	for (i=1; i<NAME_LEN; i++) {
		if (*queries[idx][i] == L'\0') break;
		if (i != 1) wcscat(queryStrBuilder, L", ");
		wcscat(queryStrBuilder, queries[idx][i]);
	}
	wcscat(queryStrBuilder, L" from ");
	wcscat(queryStrBuilder, queries[idx][0]);

	queryStr = SysAllocString(queryStrBuilder);
	printf("\n\n[*] Query: %S\n", queryStr);


	pSvc->ExecQuery(wqlStr, queryStr, flags, NULL, &pEnum);

	iter_keys(pEnum, idx);

	pEnum->Release();

	return;
}


void iter_keys(IEnumWbemClassObject *pEnum, int idx) {
	int i;
	HRESULT hres;
	ULONG uReturn, returns;
	BSTR keyStr;
	IWbemClassObject *pWbemObj;
	VARIANT v;
	CIMTYPE cimType;

	returns = 0;
	while (1) {
		hres = pEnum->Next(WBEM_INFINITE, 1, &pWbemObj, &uReturn);
		if (uReturn < 1) break;
		returns += uReturn;
		printf("\n[*] %lu\n", returns);

		for (i=1; i<NAME_LEN; i++) {
			if (*queries[idx][i] == L'\0') break;
			keyStr = SysAllocString(queries[idx][i]);

			VariantInit(&v);
			hres = pWbemObj->Get(keyStr, 0, &v, &cimType, NULL);

			if (FAILED(hres)) {
				printf("[!] Failed to retrieve key %S: 0x%08lx\n", keyStr, hres);
				continue;
			}

			printf("[*] cimType: %ld, vt: %d\n", cimType, v.vt);
			if (v.vt == VT_BSTR || cimType == CIM_STRING) {
				printf("[*] %S : %S\n", keyStr, v.bstrVal);
			} else {
				printf("[*] %S : not string. Type: %ld\n", keyStr, cimType);
			}

			VariantClear(&v);
		}
		pWbemObj->Release();
	}

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
		execute_query(i);

	pSvc->Release();
	CoUninitialize();

	return 0;
}
