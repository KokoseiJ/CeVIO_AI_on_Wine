#pragma comment(lib, "iphlpapi.lib")

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iphlpapi.h>


unsigned long dump_adapters(void* buffer, unsigned long* bufsize) {
	return GetAdaptersAddresses(
		AF_UNSPEC,
		GAA_FLAG_INCLUDE_WINS_INFO | GAA_FLAG_INCLUDE_GATEWAYS,
		NULL,
		(PIP_ADAPTER_ADDRESSES) buffer,
		bufsize
	);
}


int main() {
    unsigned long bufsize;
    void* buffer, * out2;

    char* out;
    size_t outlen, out2len;

    dump_adapters(NULL, &bufsize);
    buffer = malloc(bufsize);
    dump_adapters(buffer, &bufsize);

    return 0;
}
