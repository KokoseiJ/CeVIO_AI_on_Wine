#include "wminet.hpp"

HRESULT (*orig_ExecQueryWmi)(BSTR, BSTR, int, IWbemContext *, IEnumWbemClassObject **, int, int, IWbemServices *, BSTR, int *, BSTR) = NULL;
HRESULT (*orig_CloneEnumWbemClassObject)(IEnumWbemClassObject **, DWORD, DWORD, IEnumWbemClassObject *, BSTR, BSTR, BSTR);


HRESULT hook_ExecQueryWmi(
	BSTR strQueryLanguage,
	BSTR strQuery,
	int lFlags,
	IWbemContext *pCtx,
	IEnumWbemClassObject **ppEnum, //OUT
	int impLevel,
	int authnLevel,
	IWbemServices *pCurrentNamespace,
	BSTR *strUser,
	int *strPassword,
	BSTR *strAuthority
) {

	char funcname[] = "hook_ExecQueryWmi";
	debug_info(funcname,
		"Called. lang: %S | query: %S | flags: %#010x | implevel: %d | authnlevel: %d | ppEnum: %p",
		strQueryLanguage, strQuery, lFlags, impLevel, authnLevel, *ppEnum);

	//*ppEnum = new CeVIOEnumWbemClassObject();
	*ppEnum = new CeVIOProxyEnumWbemClassObject(strQuery);

	return S_OK;

	//return orig_ExecQueryWmi(strQueryLanguage, strQuery, lFlags, pCtx, ppEnum, impLevel, authnLevel,
	//			 pCurrentNamespace, strUser, strPassword, strAuthority);
}

// ManagementObjectCollection.GetEnumerator() Clones using CloneEnumWbemClassObject
HRESULT hook_CloneEnumWbemClassObject (
   IEnumWbemClassObject **ppEnum,
   DWORD authLevel,
   DWORD impLevel,
   IEnumWbemClassObject *pCurrentEnumWbemClassObject,
   BSTR strUser,
   BSTR strPassword,
   BSTR strAuthority
) {
	char funcname[] = "hook_CloneEnumWbemClassObject";
	debug_info(funcname, "Called.");

	//return orig_CloneEnumWbemClassObject(ppEnum, authLevel, impLevel, pCurrentEnumWbemClassObject,
	//	strUser, strPassword, strAuthority);
	return pCurrentEnumWbemClassObject->Clone(ppEnum);
}


int init_wminet() {
	HMODULE wminet_handle;
	FARPROC ptr_ExecQueryWmi, ptr_CloneEnumWbemClassObject;
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

	ptr_CloneEnumWbemClassObject = GetProcAddress(wminet_handle, "CloneEnumWbemClassObject");
	if (ptr_CloneEnumWbemClassObject == NULL) {
		error_message("Failed to get address for CloneEnumWbemClassObject!");
		return 1;
	}

	if (MH_CreateHook(
			reinterpret_cast<LPVOID*>(ptr_ExecQueryWmi),
			reinterpret_cast<LPVOID*>(&hook_ExecQueryWmi),
			reinterpret_cast<LPVOID*>(&orig_ExecQueryWmi)) != MH_OK) {
		error_message("Failed to create ExecQueryWmi hook!");
		return 1;
	}

	if (MH_CreateHook(
			reinterpret_cast<LPVOID*>(ptr_CloneEnumWbemClassObject),
			reinterpret_cast<LPVOID*>(&hook_CloneEnumWbemClassObject),
			reinterpret_cast<LPVOID*>(&orig_CloneEnumWbemClassObject)) != MH_OK) {
		error_message("Failed to create CloneEnumWbemClassObject hook!");
		return 1;
	}


	if (init_wbemsvc()) {
		error_message("Failed to initialize IWbemService!");
		return 1;
	}

	return 0;
}

