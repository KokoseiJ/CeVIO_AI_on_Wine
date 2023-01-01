#include "misc.h"

const char *wine_get_version() {
	HMODULE ntdll = GetModuleHandle("ntdll.dll");
	const char *returnval;

	if (!ntdll) return NULL;

	const char * (*orig_wine_get_version)() = (void *)GetProcAddress(ntdll, "wine_get_version");

	returnval = orig_wine_get_version ? orig_wine_get_version() : NULL;

	return returnval;
}


const char *wine_get_host_version() {
	HMODULE ntdll = GetModuleHandle("ntdll.dll");
	const char *returnval;

	if (!ntdll) return "(null)_nontdll";

	const char * (*orig_wine_get_host_version)() = (void *)GetProcAddress(ntdll, "wine_get_host_version");

	returnval = orig_wine_get_host_version ? orig_wine_get_host_version() : "(null)";

	return returnval;
}

