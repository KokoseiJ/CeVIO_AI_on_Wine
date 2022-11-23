#ifdef __MINGW32__
#include <initguid.h>
#else
#pragma comment(lib, "wbemuuid.lib")
#endif

#include <wbemcli.h>
#include <windows.h>
#include <stdio.h>

IWbemServices *initialize_wbem()
{
	HRESULT hres;
	BSTR tempBstr;
	IWbemLocator *pLoc = NULL;
	IWbemServices *pSvc = NULL;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres))
	{
		printf("[!] Failed to initialize COM: 0x%08x\n", hres);
		return NULL;
	}


	hres =  CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL
	);

	if (FAILED(hres))
	{
		printf("[!] Failed to initialize Security: x=0x%08x\n", hres);
		CoUninitialize();
		return NULL;
	}


	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator,
		(LPVOID *) &pLoc
	);
 
	if (FAILED(hres))
	{
		printf("[!] Failed to create IWbemLocator: 0x%08x\n", hres);
		CoUninitialize();
		return NULL;
	}


	tempBstr = SysAllocString(L"ROOT\\CIMV2");
	hres = pLoc->ConnectServer(
		 tempBstr,
		 NULL,
		 NULL,
		 0,
		 NULL,
		 0,
		 0,
		 &pSvc
	);
	SysFreeString(tempBstr);
	
	if (FAILED(hres))
	{
		printf("[!] Failed to connect to CIMV2 WMI namespace: 0x%08x\n", hres);
		pLoc->Release();
		CoUninitialize();
		return NULL;
	}


	hres = CoSetProxyBlanket(
	   pSvc,
	   RPC_C_AUTHN_WINNT,
	   RPC_C_AUTHZ_NONE,
	   NULL,
	   RPC_C_AUTHN_LEVEL_CALL,
	   RPC_C_IMP_LEVEL_IMPERSONATE,
	   NULL,
	   EOAC_NONE
	);

	if (FAILED(hres))
	{
		printf("[!] Failed to set Proxy Blanket: 0x%08x\n", hres);
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return NULL;
	}

	pLoc->Release();

	return pSvc;
}


int main() {
	HRESULT hres;
	BSTR wqlStr, queryStr;
	IWbemServices *pSvc;
	IEnumWbemClassObject* pEnumerator = NULL;

	wqlStr = SysAllocString(L"WQL");
	queryStr = SysAllocString(L"select Caption from Win32_OperatingSystem");

	pSvc = initialize_wbem();
	if (pSvc == NULL) {
		printf("[!] Failed to initialize Wbem client.\n");
		return 1;
	}

	hres = pSvc->ExecQuery(
		wqlStr,
		queryStr,
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);
	
	if (FAILED(hres))
	{
		printf("[!] Failed to query stuff\n");
		pSvc->Release();
		CoUninitialize();
		return 1;
	}

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
   
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		VariantInit(&vtProp);
		// Get the value of the Name property
		hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
		printf("[*] Win32_OperatingSystem::Caption : %S\n", vtProp.bstrVal);
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	// Cleanup
	// ========
	
	pSvc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return 0;   // Program successfully completed.

}
