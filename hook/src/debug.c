#include "debug.h"

HANDLE hConsole = NULL;
char *window_title = "CeVIO AI on Wine";


DWORD debug_print(char *string) {
#ifdef DEBUG
    size_t string_length;
    DWORD written_chars = 0;
    
    if (string == NULL) return 0;
    if (hConsole == NULL) return 0;

    string_length = strlen(string);
    
    WriteConsole(hConsole, string, string_length, &written_chars, NULL);

    return written_chars;
#else
    return 0;
#endif
}

DWORD debug_printf(char *fmt, ...) {
#ifdef DEBUG
    char buffer[1024];
    va_list args;
    
    if (fmt == NULL) return 0;
    if (hConsole == NULL) return 0;
    
    va_start(args, fmt);
    vsnprintf(buffer, 1024, fmt, args);

    return debug_print(buffer);
#else
    return 0;
#endif
}

int init_console() {
#ifdef DEBUG
    AllocConsole();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (hConsole == NULL) {
        return 1;
    }
#endif
    return 0;
}


int info_message(char *content) {
    debug_printf("[*] (info_message) %s\n", content);
    return MessageBoxA(NULL, content, window_title, MB_OK | MB_ICONINFORMATION);
}

int error_message(char *content) {
    debug_printf("[!] (error_message) %s\n", content);
    return MessageBoxA(NULL, content, window_title, MB_OK | MB_ICONERROR);
}

