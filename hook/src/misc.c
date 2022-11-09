#include "misc.h"

const char *wine_get_version() {
    HMODULE ntdll = GetModuleHandle("ntdll.dll");

    if (!ntdll) return NULL;

    const char * (*orig_wine_get_version)() = (void *)GetProcAddress(ntdll, "wine_get_version");

    return orig_wine_get_version ? orig_wine_get_version() : NULL;
}

