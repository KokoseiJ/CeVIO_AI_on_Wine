#include "wminet.h"

HRESULT (*orig_ExecQueryWmi)(const BSTR, const BSTR, int, IWbemContext *, IEnumWbemClassObject **, int, int, IWbemServices *, const BSTR, int *, const BSTR) = NULL;


HRESULT hook_ExecQueryWmi(
	const BSTR strQueryLanguage,
	const BSTR strQuery,
	int lFlags,
	IWbemContext *pCtx,
	IEnumWbemClassObject **ppEnum, //OUT
	int impLevel,
	int authnLevel,
	IWbemServices *pCurrentNamespace,
	const BSTR *strUser,
	int *strPassword,
	const BSTR *strAuthority
) {
	char funcname[] = "hook_ExecQueryWmi";
	debug_info(funcname,
		"Called. lang: %S | query: %S | flags: %#010x | implevel: %d | authnlevel: %d | "
		"user: %S | pw: 0x%08x | authority: %S",
		strQueryLanguage, strQuery, lFlags, impLevel, authnLevel, strUser, strPassword, strAuthority);

	return orig_ExecQueryWmi(strQueryLanguage, strQuery, lFlags, pCtx, ppEnum, impLevel, authnLevel,
				 pCurrentNamespace, strUser, strPassword, strAuthority);
}

int init_wminet() {
	HMODULE wminet_handle;
	FARPROC ptr_ExecQueryWmi;
	const char wminet_path[] = "C:\\windows\\Microsoft.NET\\Framework64\\v4.0.30319\\WMINet_Utils.dll";

	wminet_handle = LoadLibraryA(wminet_path);
	if (wminet_handle == NULL) {
		error_message("Failed to load WMINet_Utils.dll!");
		return 1;
	}

	ptr_ExecQueryWmi = GetProcAddress(wminet_handle, "ExecQueryWmi");
	if (ptr_ExecQueryWmi == NULL) {
		error_message("Failed to get address for ExecQueryWmi!");
		return 1;
	}

	if (MH_CreateHook(ptr_ExecQueryWmi, &hook_ExecQueryWmi, &orig_ExecQueryWmi) != MH_OK) {
		error_message("Failed to create ExecQueryWmi hook!");
		return 1;
	}

	return 0;
}

