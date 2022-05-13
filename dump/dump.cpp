#pragma comment(lib, "iphlpapi.lib")

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <ws2def.h>
#include "base64.h"

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
    char stringBuf[128], *returnBuf;
    FILE* pPipe;

    pPipe = _popen("wmic logicaldisk get volumeserialnumber", "rt");

    fgets(stringBuf, 128, pPipe);
    fgets(stringBuf, 128, pPipe);

    trim_space(stringBuf);

    returnBuf = (char*)calloc(strlen(stringBuf) + 1, sizeof(char));
    if (returnBuf == NULL) {
        printf("[*] Failed to allocate returnBuf.\n");
        exit(1);
    }

    strcpy_s(returnBuf, (strlen(stringBuf) + 1) * sizeof(char), stringBuf);

    printf("%s\n", returnBuf);

    return returnBuf;
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
    unsigned long bufsize;
    void* buffer;
    PIP_ADAPTER_ADDRESSES addrPtr;
    FILE* pFile;
    char stringBuf[256], * strPtr;
    uint8_t i = 0;

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

    fclose(pFile);

    return 0;
}
