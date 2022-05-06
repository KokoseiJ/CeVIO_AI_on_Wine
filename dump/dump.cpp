#pragma comment(lib, "iphlpapi.lib")

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include "base64.h"


unsigned long dump_adapters(void* buffer, unsigned long* bufsize) {
	return GetAdaptersAddresses(
		AF_UNSPEC,
		GAA_FLAG_INCLUDE_WINS_INFO | GAA_FLAG_INCLUDE_GATEWAYS,
		NULL,
		(PIP_ADAPTER_ADDRESSES) buffer,
		bufsize
	);
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
    FILE* pFile;

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

    write_dump(pFile, "GetAdaptersAddresses", buffer, bufsize);

    fclose(pFile);

    return 0;
}
