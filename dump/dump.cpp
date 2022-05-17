#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "Wbemuuid.lib")

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <ws2def.h>
#include <WbemCli.h>
#include <comutil.h>
#include "base64.h"

IWbemServices* wbemServices = NULL;

char* trim_space(char* string) {
    while (*string != NULL) {
        if (*string == ' ' || *string == '\r' || *string == '\n') {
            *string = '\0';
            return string;
        }
        string++;
    }
    return string;
}


void prepare_com() {
    HRESULT hres;
    IWbemLocator* pLoc = 0;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);

    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_IMP_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );

    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &wbemServices
    );

    hres = CoSetProxyBlanket(
        wbemServices,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    pLoc->Release();
}

void uninit_com() {
    wbemServices->Release();
    CoUninitialize();
}


unsigned long dump_adapters(void* buffer, unsigned long* bufsize) {
	return GetAdaptersAddresses(
		AF_UNSPEC,
		GAA_FLAG_INCLUDE_WINS_INFO | GAA_FLAG_INCLUDE_GATEWAYS,
		NULL,
		(PIP_ADAPTER_ADDRESSES) buffer,
		bufsize
	);
}

char* dump_hdserial() {
    HRESULT hres;
    IEnumWbemClassObject* enumWbemObj = NULL;
    IWbemClassObject* wbemClassObj = NULL;
    ULONG uReturn;
    VARIANT value;
    size_t strsize;
    char* returnBuf;

    hres = wbemServices->ExecQuery(
        _bstr_t("WQL"),
        _bstr_t("SELECT VolumeSerialNumber FROM Win32_LogicalDisk"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &enumWbemObj
    );

    hres = enumWbemObj->Next(WBEM_INFINITE, 1, &wbemClassObj, &uReturn);
    if (uReturn != 1) {
        printf("[*] Error while dumping hdserial.\n");
        exit(1);
    }

    hres = wbemClassObj->Get(L"VolumeSerialNumber", 0, &value, 0, 0);
    
    strsize = wcslen(value.bstrVal);
    returnBuf = (char*)calloc(strsize + 1, sizeof(char));

    wcstombs_s(
        &strsize,
        returnBuf,
        (strsize + 1) * sizeof(char),
        value.bstrVal,
        strsize * sizeof(char)
    );

    return returnBuf;
}


IEnumWbemClassObject* dump_sounddev() {
    HRESULT hres;
    IEnumWbemClassObject* enumWbemObj;
    hres = wbemServices->ExecQuery(
        _bstr_t("WQL"),
        _bstr_t("SELECT DeviceID, Caption FROM Win32_SoundDevice"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &enumWbemObj
    );

    return enumWbemObj;
}


void write_dump(FILE* pFile, const char* name, void* buffer, size_t bufsize) {
    char* b64out;
    size_t b64outlen;
    
    b64outlen = b64enclen(bufsize);
    b64out = (char*)calloc(b64outlen + 1, sizeof(char));

    b64encode(buffer, bufsize, b64out, b64outlen);

    fprintf(
        pFile,
        "%s\n%s\n",
        name, b64out
    );
    
    printf(
        "%s\n%zu\n",
        name, bufsize
    );

    free(b64out);

    return;
}


int main() {
    FILE* pFile;
    char stringBuf[256], * strPtr;
    uint8_t i = 0;

    unsigned long bufsize;
    void* buffer;
    
    HRESULT hres;
    PIP_ADAPTER_ADDRESSES addrPtr;
    IEnumWbemClassObject* enumWbemObj = NULL;
    IWbemClassObject* wbemClassObj = NULL;
    ULONG uReturn;
    VARIANT vtval;
    size_t strsize;

    prepare_com();

    fopen_s(&pFile, "dump.cfg", "w");
    if (pFile == NULL) {
        printf("[*] Failed to open file.\n");
        return 1;
    }

    dump_adapters(NULL, &bufsize);
    buffer = malloc(bufsize);
    if (buffer == NULL) {
        printf("[*] Failed to allocate PIP_ADAPTER_ADDRESSES buffer.\n");
        return 1;
    }
    dump_adapters(buffer, &bufsize);

    i = 0;
    addrPtr = (PIP_ADAPTER_ADDRESSES)buffer;
    while (addrPtr != NULL) {
        sprintf_s(stringBuf, 256 * sizeof(char), "adapter%u", i);
        write_dump(pFile, stringBuf, addrPtr, addrPtr->Length);
        sprintf_s(stringBuf, 256 * sizeof(char), "adapter%uname", i);
        write_dump(pFile, stringBuf, addrPtr->AdapterName, (strlen(addrPtr->AdapterName) + 1) * sizeof(char));
        sprintf_s(stringBuf, 256 * sizeof(char), "adapter%udesc", i);
        write_dump(pFile, stringBuf, addrPtr->Description, (wcslen(addrPtr->Description) + 1) * sizeof(wchar_t));
        if (addrPtr->Dhcpv4Server.iSockaddrLength != 0) {
            sprintf_s(stringBuf, 256 * sizeof(char), "adapter%uipv4", i);
            write_dump(pFile, stringBuf, addrPtr->Dhcpv4Server.lpSockaddr, addrPtr->Dhcpv4Server.iSockaddrLength);
        }
        if (addrPtr->Dhcpv6Server.iSockaddrLength != 0) {
            sprintf_s(stringBuf, 256 * sizeof(char), "adapter%uipv6", i);
            write_dump(pFile, stringBuf, addrPtr->Dhcpv6Server.lpSockaddr, addrPtr->Dhcpv6Server.iSockaddrLength);
        }
        printf("[*] %s %d %d\n", addrPtr->AdapterName, addrPtr->Dhcpv4Server.iSockaddrLength, addrPtr->Dhcpv6Server.iSockaddrLength);
        i++;
        addrPtr = addrPtr->Next;
    }

    write_dump(pFile, "adapterlen", &i, sizeof(uint8_t));

    strPtr = dump_hdserial();
    write_dump(pFile, "hdPrimarySerial", strPtr, (strlen(strPtr) + 1) * sizeof(char));
    
    i = 0;
    enumWbemObj = dump_sounddev();
    while (enumWbemObj) {
        hres = enumWbemObj->Next(WBEM_INFINITE, 1, &wbemClassObj, &uReturn);
        if (uReturn == 0) break;

        hres = wbemClassObj->Get(L"DeviceID", 0, &vtval, 0, 0);
        strsize = wcslen(vtval.bstrVal);
        strPtr = (char*)calloc(strsize + 1, sizeof(char));
        wcstombs_s(&strsize, strPtr, (strsize + 1) * sizeof(char), vtval.bstrVal, strsize * sizeof(char));
        sprintf_s(stringBuf, 256 * sizeof(char), "sound%uid", i);
        write_dump(pFile, stringBuf, strPtr, (strsize) * sizeof(char));
        free(strPtr);

        hres = wbemClassObj->Get(L"Caption", 0, &vtval, 0, 0);
        strsize = wcslen(vtval.bstrVal);
        strPtr = (char*)calloc(strsize + 1, sizeof(char));
        wcstombs_s(&strsize, strPtr, (strsize + 1) * sizeof(char), vtval.bstrVal, strsize * sizeof(char));
        sprintf_s(stringBuf, 256 * sizeof(char), "sound%ucaption", i);
        write_dump(pFile, stringBuf, strPtr, (strsize) * sizeof(char));
        free(strPtr);

        i++;
    }
    write_dump(pFile, "soundlen", &i, sizeof(uint8_t));

    fclose(pFile);

    uninit_com();

    return 0;
}
