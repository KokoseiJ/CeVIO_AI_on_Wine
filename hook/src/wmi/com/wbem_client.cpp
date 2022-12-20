#include "wbem_client.hpp"

IWbemServices *pSvc = NULL;


int init_wbemsvc() {
    HRESULT hres;
    BSTR tempBstr;
    IWbemLocator *pLoc = NULL;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED); 
    if (FAILED(hres)) {
        //printf("[!] Failed to initialize COM: 0x%08lx\n", hres);
        return 1;
    }

    hres =  CoInitializeSecurity(
        NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL
    );
    if (FAILED(hres)) {
        //printf("[!] Failed to initialize Security: 0x%08lx\n", hres);
        CoUninitialize();
        return 1;
    }

    hres = CoCreateInstance(
        CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator,
        (LPVOID *) &pLoc
    );
    if (FAILED(hres)) {
        //printf("[!] Failed to create IWbemLocator: 0x%08lx\n", hres);
        CoUninitialize();
        return 1;
    }

    tempBstr = SysAllocString(L"ROOT\\CIMV2");
    hres = pLoc->ConnectServer(tempBstr, NULL, NULL, 0, 0, 0, 0, &pSvc);
    SysFreeString(tempBstr);
    
    if (FAILED(hres)) {
        //printf("[!] Failed to connect to CIMV2 WMI namespace: 0x%08lx\n", hres);
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    hres = CoSetProxyBlanket(
        pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE
    );
    if (FAILED(hres)) {
        //printf("[!] Failed to set Proxy Blanket: 0x%08lx\n", hres);
        pSvc->Release();
        pLoc->Release();     
        CoUninitialize();
        return 1;
    }

    pLoc->Release();

    return 0;
}


IEnumWbemClassObject *get_pEnum(BSTR query, long flags) {
    static const char funcname[] = "get_pEnum";
    static const BSTR wqlStr = SysAllocString(L"WQL");

    HRESULT hres;
    IEnumWbemClassObject *pEnum = NULL;

    debug_info(funcname, "Called. Query: %S | flags: 0x%08lx", query, flags);

    if (!pSvc) {
        debug_error(funcname, "pSvc not initialized!");
        return NULL;
    }

    hres = pSvc->ExecQuery(wqlStr, query, flags, NULL, &pEnum);

    if (FAILED(hres)) {
        debug_error(funcname, "ExecQuery failed. hres: 0x%08lx", hres);
        return NULL;
    }

    debug_info(funcname, "pEnum: %p", pEnum);

    return pEnum;
}
