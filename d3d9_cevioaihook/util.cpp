#include "util.h"

HANDLE consoneHandle;


// =============== Debug console ===============
DWORD print(const char* stringBuf) {
	DWORD stringBufLen;
	DWORD written_chars;

	stringBufLen = static_cast<DWORD>(strlen(stringBuf));

	WriteConsole(
		consoneHandle,
		stringBuf,
		stringBufLen,
		&written_chars,
		NULL
	);

	//printf(stringBuf);

	return written_chars;
}

HANDLE setup_console() {
	char stringBuf[256];

	AllocConsole();
	SetConsoleTitle("CeVIO AI INJECTION DEBUG CONSOLE");
	consoneHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	sprintf_s(
		stringBuf,
		128 * sizeof(char),
		"[*] (setup_console) \t| Console Attached. StdHandle: %d\n", (int)consoneHandle
	);
	print(stringBuf);

	return stdout;
}


// =============== Utility Function Codes ===============
char* splitstr(char** str, char delim) {
	char* rtnstr, * splitptr;
	rtnstr = *str;
	splitptr = strchr(*str, delim);
	if (splitptr == NULL) {
		*str = NULL;
		print("[*] (splitstr) \t| This should've never happened. what have you done??\n");
	}
	else {
		*splitptr = '\0';
		*str = splitptr + 1;
	}
	return rtnstr;
}

long int flen(FILE* pFile) {
	long pos, end;

	pos = ftell(pFile);
	fseek(pFile, 0, SEEK_END);
	end = ftell(pFile);
	fseek(pFile, pos, SEEK_SET);

	return end - pos;
}
