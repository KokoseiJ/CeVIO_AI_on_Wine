#ifdef __MINGW32__
#include <initguid.h>
#else
#pragma comment(lib, "wbemuuid.lib")
#endif

extern "C" {
#include <kcnf/kcnf.h>
#include <kcnf/base64.h>
}
#include <wbemcli.h>
#include <windows.h>
#include <stdio.h>

#define LEN(arr) ((int) (sizeof(arr) / sizeof(arr[0])))
#define QUERY_LEN 128
#define NAME_LEN 6
#define NAME_STR_LEN 30

IWbemServices *pSvc = NULL;

char queries[][NAME_LEN][NAME_STR_LEN] {
	{"Win32_OperatingSystem", "Caption", "CSDVersion", "Version"},
	{"Win32_OperatingSystem", "TotalVisibleMemorySize", "FreePhysicalMemory"},
	{"Win32_Processor", "Name", "CurrentClockSpeed"},
	{"Win32_VideoController", "Caption", "DriverVersion"},
	{"Win32_SoundDevice", "Caption", "DeviceID"},
	{"Win32_NetworkAdapter", "Name", "NetConnectionID", "AdapterType", "PhysicalAdapter", "NetEnabled"},
	{"Win32_LogicalDisk", "VolumeSerialNumber"}
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

Config *iter_keys(IEnumWbemClassObject *, int);
void execute_query(int idx) {
	static const BSTR wqlStr = SysAllocString(L"WQL");
	static const long flags = WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY;

	int i;
	HRESULT hres;
	char queryStrBuilder[QUERY_LEN] = "select ";
	wchar_t queryWstr[QUERY_LEN];
	BSTR queryStr;

	IEnumWbemClassObject *pEnum = NULL;

	for (i=1; i<NAME_LEN; i++) {
		if (*queries[idx][i] == '\0') break;
		if (i != 1) strcat(queryStrBuilder, ", ");
		strcat(queryStrBuilder, queries[idx][i]);
	}
	strcat(queryStrBuilder, " from ");
	strcat(queryStrBuilder, queries[idx][0]);
	if (strcmp(queries[idx][0], "Win32_LogicalDisk") == 0) strcat(queryStrBuilder, " where DeviceID=\"C:\"");

	mbstowcs(queryWstr, queryStrBuilder, QUERY_LEN);

	queryStr = SysAllocString(queryWstr);
	printf("\n\n[*] Query: %S\n", queryWstr);

	hres = pSvc->ExecQuery(wqlStr, queryStr, flags, NULL, &pEnum);
	if (FAILED(hres)) {
		printf("[!] Failed to query: 0x%08lx\n", hres);
		return;
	}

	iter_keys(pEnum, idx);

	pEnum->Release();

	return;
}


Config *iter_keys(IEnumWbemClassObject *pEnum, int idx) {
	int i;
	HRESULT hres;
	ULONG uReturn, returns;

	wchar_t keyWstr[NAME_STR_LEN];
	BSTR keyStr;

	size_t b64inlen, b64outlen;
	char *b64out;

	Config *config = NULL, *child = NULL, *value = NULL;
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
			mbstowcs(keyWstr, queries[idx][i], NAME_STR_LEN);
			keyStr = SysAllocString(keyWstr);

			VariantInit(&v);
			hres = pWbemObj->Get(keyStr, 0, &v, &cimType, NULL);

			if (FAILED(hres)) {
				printf("[!] Failed to retrieve key %S: 0x%08lx\n", keyStr, hres);
				continue;
			}

			printf("[*] cimType: %ld, vt: %d\n", cimType, v.vt);

			value = config_value_put(NULL, &cimType, sizeof(CIMTYPE));
			child = config_pair_put(NULL, "cimtype", value);

			switch (cimType) {
			case CIM_STRING:
			case CIM_UINT64:
				printf("[*] %S : %S\n", keyStr, v.bstrVal);
				value = config_value_put(NULL, v.bstrVal, (SysStringLen(v.bstrVal) + 1) * sizeof(OLECHAR));
				child = config_pair_put(child, "bstr", value);
				break;
			case CIM_UINT32:
				printf("[*] %S: %u\n", keyStr, v.uintVal);
				break;
			case CIM_BOOLEAN:
				printf("[*] %S : %s\n", keyStr, v.boolVal ? "true" : "false");
				break;
			default:
				printf("[*] %S : Unknown Type. Type: %ld\n", keyStr, cimType);
			}

			VariantClear(&v);
		}
		pWbemObj->Release();
	}

	return NULL;
}


int main() {
	int i = 0;
	Config *config = NULL;

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
