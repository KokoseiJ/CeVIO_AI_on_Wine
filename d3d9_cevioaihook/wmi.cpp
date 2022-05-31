#include "wmi.h"


// =============== WMI Class implementations ===============

// =============== HookWbemClassObject ===============
HookWbemClassObject::HookWbemClassObject(Linkedlist* llist, const wchar_t* relpath) {
	print("[*] (HookWbemClassObject::cctor) \t| Object created.\n");
	this->llist = llist;
	this->relpath = relpath;
}

ULONG HookWbemClassObject::AddRef() {
	char stringBuf[256];
	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (HookWbemClassObject::AddRef) \t| Called. refcount: %lu\n",
		++refcount
	);
	print(stringBuf);

	return refcount;
}
HRESULT HookWbemClassObject::QueryInterface(REFIID riid, void** ppvObject) {
	print("[*] (HookWbemClassObject::QueryInterface) \t| Called.\n");
	if (ppvObject == NULL) return E_POINTER;
	if (riid == IID_IUnknown || riid == IID_IWbemClassObject) {
		print("[*] (HookWbemClassObject::QueryInterface) \t| RIID match found.\n");
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	print("[*] (HookWbemClassObject::QueryInterface) \t| RIID match not found.\n");
	return E_NOINTERFACE;
}
ULONG HookWbemClassObject::Release() {
	char stringBuf[256];
	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (HookWbemClassObject::Release) \t| Called. refcount: %lu\n",
		--refcount
	);
	print(stringBuf);

	return refcount;
}

HRESULT HookWbemClassObject::BeginEnumeration(long lEnumFlags) {
	print("[*] (HookWbemClassObject::BeginEnumeration) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::BeginMethodEnumeration(long lEnumFlags) {
	print("[*] (HookWbemClassObject::BeginMethodEnumeration) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::Clone(IWbemClassObject** ppCopy) {
	print("[*] (HookWbemClassObject::Clone) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::CompareTo(long lFlags, IWbemClassObject* pCompareTo) {
	print("[*] (HookWbemClassObject::CompareTo) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::Delete(LPCWSTR wszName) {
	print("[*] (HookWbemClassObject::Delete) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::DeleteMethod(LPCWSTR wszName) {
	print("[*] (HookWbemClassObject::DeleteMethod) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::EndEnumeration(void) {
	print("[*] (HookWbemClassObject::EndEnumeration) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::EndMethodEnumeration(void) {
	print("[*] (HookWbemClassObject::EndMethodEnumeration) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::Get(LPCWSTR wszName, long lFlags, VARIANT* pVal, CIMTYPE* pType, long* plFlavor) {
	char stringBuf[256], mbsbuf[128];
	Linkedlist* listobj;
	size_t i;

	wcstombs_s(&i, mbsbuf, 128, wszName, 128 * sizeof(char));

	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (HookWbemClassObject::Get) \t| Called. Name: %s\n",
		mbsbuf
	);
	print(stringBuf);

	if (strcmp(mbsbuf, "__GENUS") == 0) {
		*pType = CIM_SINT32;
		VariantInit(pVal);
		if (VariantChangeType(pVal, pVal, NULL, *pType) != S_OK) {
			print("[*] (HookWbemClassObject::Get) \t| VariantChangeType failed.\n");
			exit(1);
		}

		pVal->intVal = WBEM_GENUS_INSTANCE;

		return WBEM_S_NO_ERROR;
	}
	else if (strcmp(mbsbuf, "__RELPATH") == 0) {
		*pType = CIM_STRING;
		VariantInit(pVal);
		if (VariantChangeType(pVal, pVal, NULL, *pType) != S_OK) {
			print("[*] (HookWbemClassObject::Get) \t| VariantChangeType failed.\n");
			exit(1);
		}

		pVal->bstrVal = SysAllocString(this->relpath);

		return WBEM_S_NO_ERROR;
	}

	listobj = llist_get(this->llist, mbsbuf);

	if (listobj == NULL) {
		print("[*] (HookWbemClassObject::Get) \t| Entry not found.\n");
		*pType = NULL;
		return WBEM_E_NOT_FOUND;
	}

	*pType = ((COMEnumType*)(listobj->value))->type;

	VariantInit(pVal);
	if (VariantChangeType(pVal, pVal, NULL, *pType) != S_OK) {
		print("[*] (HookWbemClassObject::Get) \t| VariantChangeType failed.\n");
		exit(1);
	}

	switch (*pType) {
	case CIM_STRING:
		pVal->bstrVal = SysAllocString((wchar_t*)(((COMEnumType*)(listobj->value))->data));
		break;
	case CIM_BOOLEAN:
		pVal->boolVal = *((bool*)(((COMEnumType*)(listobj->value))->data));
		break;
	}

	return WBEM_S_NO_ERROR;
}
HRESULT HookWbemClassObject::GetMethod(LPCWSTR wszName, long lFlags, IWbemClassObject** ppInSignature, IWbemClassObject** ppOutSignature) {
	print("[*] (HookWbemClassObject::GetMethod) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::GetMethodOrigin(LPCWSTR wszMethodName, BSTR* pstrClassName) {
	print("[*] (HookWbemClassObject::GetMethodOrigin) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::GetMethodQualifierSet(LPCWSTR wszMethod, IWbemQualifierSet** ppQualSet) {
	print("[*] (HookWbemClassObject::GetMethodQualifierSet) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::GetNames(LPCWSTR wszQualifierName, long lFlags, VARIANT* pQualifierVal, SAFEARRAY** pNames) {
	print("[*] (HookWbemClassObject::GetNames) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::GetObjectText(long lFlags, BSTR* pstrObjectText) {
	print("[*] (HookWbemClassObject::GetObjectText) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::GetPropertyOrigin(LPCWSTR wszName, BSTR* pstrClassName) {
	print("[*] (HookWbemClassObject::GetPropertyOrigin) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::GetPropertyQualifierSet(LPCWSTR wszProperty, IWbemQualifierSet** ppQualSet) {
	print("[*] (HookWbemClassObject::GetPropertyQualifierSet) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::GetQualifierSet(IWbemQualifierSet** ppQualSet) {
	print("[*] (HookWbemClassObject::GetQualifierSet) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::InheritsFrom(LPCWSTR strAncestor) {
	print("[*] (HookWbemClassObject::InheritsFrom) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::Next(long lFlags, BSTR* strName, VARIANT* pVal, CIMTYPE* pType, long* plFlavor) {
	print("[*] (HookWbemClassObject::Next) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::NextMethod(long lFlags, BSTR* pstrName, IWbemClassObject** ppInSignature, IWbemClassObject** ppOutSignature) {
	print("[*] (HookWbemClassObject::NextMethod) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::Put(LPCWSTR wszName, long lFlags, VARIANT* pVal, CIMTYPE Type) {
	print("[*] (HookWbemClassObject::Put) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::PutMethod(LPCWSTR wszName, long lFlags, IWbemClassObject* pInSignature, IWbemClassObject* pOutSignature) {
	print("[*] (HookWbemClassObject::PutMethod) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::SpawnDerivedClass(long lFlags, IWbemClassObject** ppNewClass) {
	print("[*] (HookWbemClassObject::pawnDerivedClass) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookWbemClassObject::SpawnInstance(long lFlags, IWbemClassObject** ppNewInstance) {
	print("[*] (HookWbemClassObject::spawnInstance) \t| Not implemented method called.\n");
	return E_NOTIMPL;
}


// =============== HookEnumWbemClassObject ===============
HookEnumWbemClassObject::HookEnumWbemClassObject(Linkedlist* llist, const wchar_t* relpath) {
	this->llist_list = (Linkedlist**)calloc(1, sizeof(void*));
	if (this->llist_list == NULL) {
		print("[*] (HookEnumWbemClassObject) \t| Failed to allocate buffer.\n");
		exit(1);
	}
	this->llist_list[0] = llist;
	this->relpath = relpath;
	this->llist_len = 1;
}

HookEnumWbemClassObject::HookEnumWbemClassObject(Linkedlist** llist_list, size_t len, const wchar_t* relpath) {
	this->llist_list = llist_list;
	this->llist_len = len;
	this->relpath = relpath;
}

HookEnumWbemClassObject::HookEnumWbemClassObject(Linkedlist** llist_list, size_t len, const wchar_t* relpath, ULONG index) {
	this->llist_list = llist_list;
	this->llist_len = len;
	this->relpath = relpath;
	this->index = index;
}


ULONG HookEnumWbemClassObject::AddRef() {
	char stringBuf[256];
	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (HookEnumWbemClassObject::AddRef) \t| Called. refcount: %lu\n",
		++refcount
	);
	print(stringBuf);

	return refcount;
}
HRESULT HookEnumWbemClassObject::QueryInterface(REFIID riid, void** ppvObject) {
	print("[*] (HookEnumWbemClassObject::QueryInterface) \t| Called.\n");
	if (ppvObject == NULL) return E_POINTER;
	if (riid == IID_IUnknown || riid == IID_IEnumWbemClassObject) {
		print("[*] (HookEnumWbemClassObject::QueryInterface) \t| RIID match found.\n");
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	print("[*] (HookEnumWbemClassObject::QueryInterface) \t| RIID match not found.\n");
	return E_NOINTERFACE;
}
ULONG HookEnumWbemClassObject::Release() {
	char stringBuf[256];
	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (HookEnumWbemClassObject::Release) \t| Called. refcount: %lu\n",
		--refcount
	);
	print(stringBuf);

	return refcount;
}


HRESULT HookEnumWbemClassObject::Clone(IEnumWbemClassObject** ppEnum) {
	print("[*] (HookEnumWbemClassObject::Clone) \t| Called.\n");
	*ppEnum = new HookEnumWbemClassObject(this->llist_list, this->llist_len, this->relpath, index);
	return S_OK;
}
HRESULT HookEnumWbemClassObject::Next(long lTimeout, ULONG uCount, IWbemClassObject** apObjects, ULONG* puReturned) {
	char stringBuf[256];
	int i;

	sprintf_s(
		stringBuf,
		256 * sizeof(char),
		"[*] (HookEnumWbemClassObject::Next) \t| Called. timeout: %ld, count: %lu, apobj: %p, puReturned: %p\n",
		lTimeout, uCount, apObjects, puReturned
	);
	print(stringBuf);

	*puReturned = 0;

	for (i = 0; i < uCount && this->index < this->llist_len; i++) {
		apObjects[i] = new HookWbemClassObject(this->llist_list[index], this->relpath);
		(*puReturned)++;
		index++;
	}
	if (uCount == *puReturned) return WBEM_S_NO_ERROR;

	return WBEM_S_FALSE;
}
HRESULT HookEnumWbemClassObject::NextAsync(ULONG uCount, IWbemObjectSink* pSink) {
	print("[*] (HookEnumWbemClassObject::NextAsync) \t| Not Implemented method called.\n");
	return E_NOTIMPL;
}
HRESULT HookEnumWbemClassObject::Reset() {
	print("[*] (HookEnumWbemClassObject::Reset) \t| Called.\n");
	index = 0;
	return S_OK;
}
HRESULT HookEnumWbemClassObject::Skip(long lTimeout, ULONG nCount) {
	print("[*] (HookEnumWbemClassObject::Skip) \t| Not Implemented method called.\n");
	return E_NOTIMPL;
}
