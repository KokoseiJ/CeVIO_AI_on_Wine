#include "misc.h"

const char *wine_get_version() {
	HMODULE ntdll = GetModuleHandle("ntdll.dll");

	if (!ntdll) return NULL;

	const char * (*orig_wine_get_version)() = (void *)GetProcAddress(ntdll, "wine_get_version");

	return orig_wine_get_version ? orig_wine_get_version() : NULL;
}


int wine_get_host_version(const char **sysname, const char **release) {
	HMODULE ntdll = GetModuleHandle("ntdll.dll");

	if (!ntdll) return 1;

	void (*orig_wine_get_host_version)(const char **, const char **) = (void *)GetProcAddress(ntdll, "wine_get_host_version");

	if (orig_wine_get_host_version) {
		orig_wine_get_host_version(sysname, release);
		return 0;
	} else
		return 1;
}


int is_linux() {
	char *sysname = NULL;

	wine_get_host_version(&sysname, NULL);

	return strcmp(sysname, "Linux") == 0 ? 1 : 0;
}

