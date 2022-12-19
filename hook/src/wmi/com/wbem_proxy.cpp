#include "wbem_proxy.hpp"

const char CeVIOProxyEnumWbemClassObject::classname[] = "CeVIOProxyEnumWbemClassObject";
const char CeVIOProxyWbemClassObject::classname[] = "CeVIOProxyWbemClassObject";


void init_funcname_proxy(const char *classname, char *funcname) {
	char newbuf[64];
	strcpy(newbuf, funcname);
	strcpy(funcname, classname);
	strcat(funcname, "::");
	strcat(funcname, newbuf);
	return;
}


// CeVIOProxyEnumWbemClassObject Implementation

CeVIOProxyEnumWbemClassObject::CeVIOProxyEnumWbemClassObject(BSTR query) {
	IEnumWbemClassObject *pEnum = get_pEnum(query);
	if (pEnum == NULL) {
		debug_error(this->classname, "pEnum is NULL!");
	}

	this->pEnum = pEnum;

	debug_info(this->classname, "Initialized. pEnum: %p", pEnum);
}

CeVIOProxyEnumWbemClassObject::CeVIOProxyEnumWbemClassObject(IEnumWbemClassObject *pEnum) {
	this->pEnum = pEnum;

	debug_info(this->classname, "Initialized. pEnum: %p", pEnum);
}

CeVIOProxyEnumWbemClassObject::~CeVIOProxyEnumWbemClassObject() {
	this->pEnum->Release();
	debug_info(this->classname, "Released pEnum.");
}

ULONG CeVIOProxyEnumWbemClassObject::AddRef() {
	char funcname[64] = "AddRef";
	init_funcname_proxy(this->classname, funcname);

	this->refcount++;
	debug_info(funcname, "Called. refcount: %lu", this->refcount);
	return this->refcount;
}

HRESULT CeVIOProxyEnumWbemClassObject::QueryInterface(REFIID riid, void **ppvObject) {
	char funcname[64] = "QueryInterface";
	init_funcname_proxy(this->classname, funcname);

	OLECHAR *guidString;

	debug_info(funcname, "Called.");

	if (!ppvObject) return E_INVALIDARG;

	*ppvObject = NULL;

	StringFromCLSID(riid, &guidString);
	debug_info(funcname, "riid: %S", guidString);

	if (riid == IID_IUnknown || riid == IID_IEnumWbemClassObject) {
	//if (true) {
		debug_info(funcname, "IID matched.");
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	debug_info(funcname, "No matching REFIID found!");
	return E_NOINTERFACE;
}

ULONG CeVIOProxyEnumWbemClassObject::Release() {
	char funcname[64] = "Release";
	init_funcname_proxy(this->classname, funcname);

	this->refcount--;
	debug_info(funcname, "Called. refcount: %lu", this->refcount);

	if (this->refcount == 0) {
		delete this;
		return 0;
	}

	return this->refcount;
}

HRESULT CeVIOProxyEnumWbemClassObject::Clone(IEnumWbemClassObject **ppEnum) {
	char funcname[64] = "Clone";
	init_funcname_proxy(this->classname, funcname);

	HRESULT hres;
	IEnumWbemClassObject *pEnumNew = NULL;

	debug_info(funcname, "Called.");

	hres = this->pEnum->Clone(&pEnumNew);
	if (FAILED(hres)) {
		debug_error(funcname, "Failed! hres: 0x%08lx", hres);
		return hres;
	}

	*ppEnum = new CeVIOProxyEnumWbemClassObject(pEnumNew);
	return hres;
}

HRESULT CeVIOProxyEnumWbemClassObject::Next(long lTimeout, ULONG uCount, IWbemClassObject **apObjects, ULONG *puReturned) {
	char funcname[64] = "Next";
	init_funcname_proxy(this->classname, funcname);

	HRESULT hres;
	IWbemClassObject *wbemClassObj = NULL;
	ULONG returned = 0;

	debug_error(funcname, "Semi-Stub: Fix return code. "
				"lTimeout: %ld, uCount: %ld, apObjects: %p",
				lTimeout, uCount, apObjects);

	for ((*puReturned)=0; *puReturned < uCount; (*puReturned)++) {
		hres = this->pEnum->Next(lTimeout, 1, &wbemClassObj, &returned);
		debug_info(funcname, "hres: 0x%08lx, puReturned: %ld", hres, *puReturned);
		if (FAILED(hres)) {
			debug_error(funcname, "Next() failed.");
			break;
		}
		if (returned != 1) {
			debug_info(funcname, "returned == 0, iteration ended?");
			break;
		}

		apObjects[*puReturned] = new CeVIOProxyWbemClassObject(wbemClassObj);
		if (hres == -1) { // Stub: End of iteration
			debug_info(funcname, "End of iteration reached!");
			break;
		}
	}

	debug_info(funcname, "Final hres: 0x%08lx, puReturned: %ld", hres, *puReturned);

	return hres;
}

HRESULT CeVIOProxyEnumWbemClassObject::NextAsync(ULONG uCount, IWbemObjectSink *pSink) {
	char funcname[64] = "NextAsync";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyEnumWbemClassObject::Reset() {
	char funcname[64] = "Reset";
	init_funcname_proxy(this->classname, funcname);

	debug_info(funcname, "Called. Proxying source Reset() method...");

	return this->pEnum->Reset();
}

HRESULT CeVIOProxyEnumWbemClassObject::Skip(long lTimeout, ULONG nCount) {
	char funcname[64] = "Skip";
	init_funcname_proxy(this->classname, funcname);

	debug_info(funcname, "Called. Proxying source Skip() method...");
	
	return this->pEnum->Skip(lTimeout, nCount);
}


// CeVIOProxyWbemClassObject Implementation

CeVIOProxyWbemClassObject::CeVIOProxyWbemClassObject(IWbemClassObject *pWbemClassObj) {
	if (pWbemClassObj == NULL) {
		debug_error(this->classname, "pWbemClassObj is NULL!");
	}

	this->pWbemClassObj = pWbemClassObj;
	debug_info(this->classname, "Initialized. pWbemClassObj: %p", pWbemClassObj);
}

CeVIOProxyWbemClassObject::~CeVIOProxyWbemClassObject() {
	this->pWbemClassObj->Release();
	debug_info(this->classname, "Released pWbemClassObj.");
}

ULONG CeVIOProxyWbemClassObject::AddRef() {
	char funcname[64] = "AddRef";
	init_funcname_proxy(this->classname, funcname);

	this->refcount++;
	debug_info(funcname, "Called. refcount: %lu", this->refcount);
	return this->refcount;
}

HRESULT CeVIOProxyWbemClassObject::QueryInterface(REFIID riid, void **ppvObject) {
	char funcname[64] = "QueryInterface";
	OLECHAR *guidString;

	init_funcname_proxy(this->classname, funcname);

	debug_info(funcname, "Called.");

	if (!ppvObject) return E_INVALIDARG;

	*ppvObject = NULL;

	StringFromCLSID(riid, &guidString);
	debug_info(funcname, "riid: %S", guidString);

	if (riid == IID_IUnknown || riid == IID_IWbemClassObject) {
	//if (true) {
		debug_info(funcname, "IID matched.");
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	debug_info(funcname, "No matching REFIID found!");
	return E_NOINTERFACE;
}

ULONG CeVIOProxyWbemClassObject::Release() {
	char funcname[64] = "Release";
	init_funcname_proxy(this->classname, funcname);

	if (this->refcount == 0)
		debug_error(funcname, "Called but refcount is already 0!");
	else
		this->refcount--;
	debug_info(funcname, "Called. refcount: %lu", this->refcount);

	if (this->refcount == 0) {
		delete this;
		return 0;
	}

	return this->refcount;
}

HRESULT CeVIOProxyWbemClassObject::GetQualifierSet(IWbemQualifierSet **ppQualSet) {
	char funcname[64] = "GetQualifierSet";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::Get(LPCWSTR wszName, long lFlags, VARIANT *pVal, CIMTYPE *pType, long *plFlavor) {
	char funcname[64] = "Get";
	init_funcname_proxy(this->classname, funcname);

	HRESULT hres;

	debug_info(funcname, "Called. wszname: %S | lFlags: %ld", wszName, lFlags);

	hres = this->pWbemClassObj->Get(wszName, lFlags, pVal, pType, plFlavor);

	if (FAILED(hres)) {
		debug_error(funcname, "Failed to retrieve key %S: 0x%08lx. ", wszName, hres);
	
		if (wcscmp(wszName, L"Caption") == 0) {
			debug_info(funcname, "Key: Caption, applying dummy BSTR");
			VariantInit(pVal);
			// TODO: Change these to enum constants
			pVal->vt = CIM_STRING;
			*pType = CIM_STRING;
			pVal->bstrVal = SysAllocString(L"Totally legit audio device");
			return S_OK;
		} else if (wcscmp(wszName, L"NetConnectionID") == 0) {
			debug_info(funcname, "Key: NetConnectionID, applying dummy BSTR");
			VariantInit(pVal);
			pVal->vt = CIM_STRING;
			*pType = CIM_STRING;
			pVal->bstrVal = SysAllocString(L"Totally legit network device");
			return S_OK;
		} else if (wcscmp(wszName, L"NetEnabled") == 0) {
			debug_info(funcname, "Key: NetEnabled, applying boolean value");
			VariantInit(pVal);
			pVal->vt = CIM_BOOLEAN;
			*pType = CIM_BOOLEAN;
			pVal->boolVal = true;
			return S_OK;
		} else {
			debug_error(funcname, "Unknown key, please contact developer to have it added");
		}

		return hres;
	}

	return hres;
}

HRESULT CeVIOProxyWbemClassObject::Put(LPCWSTR wszName, long lFlags, VARIANT *pVal, CIMTYPE Type) {
	char funcname[64] = "Put";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::Delete(LPCWSTR wszName) {
	char funcname[64] = "Delete";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::GetNames(LPCWSTR wszQualifierName, long lFlags, VARIANT *pQualifierVal, SAFEARRAY * *pNames) {
	char funcname[64] = "GetNames";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::BeginEnumeration(long lEnumFlags) {
	char funcname[64] = "BeginEnumeration";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::Next(long lFlags, BSTR *strName, VARIANT *pVal, CIMTYPE *pType, long *plFlavor) {
	char funcname[64] = "Next";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::EndEnumeration( void) {
	char funcname[64] = "EndEnumeration";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::GetPropertyQualifierSet(LPCWSTR wszProperty, IWbemQualifierSet **ppQualSet) {
	char funcname[64] = "GetPropertyQualifierSet";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}
;
HRESULT CeVIOProxyWbemClassObject::Clone(IWbemClassObject **ppCopy) {
	char funcname[64] = "Clone";
	init_funcname_proxy(this->classname, funcname);

	HRESULT hres;
	IWbemClassObject *pNewWbemClassObj = NULL;

	debug_info(funcname, "Called.");
	
	hres = this->pWbemClassObj->Clone(&pNewWbemClassObj);

	if (FAILED(hres)) {
		debug_error(funcname, "Failed! hres: 0x%08lx", hres);
		return hres;
	}

	*ppCopy = new CeVIOProxyWbemClassObject(pNewWbemClassObj);
	return hres;
}

HRESULT CeVIOProxyWbemClassObject::GetObjectText(long lFlags, BSTR *pstrObjectText) {
	char funcname[64] = "GetObjectText";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::SpawnDerivedClass(long lFlags, IWbemClassObject **ppNewClass) {
	char funcname[64] = "SpawnDerivedClass";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::SpawnInstance(long lFlags, IWbemClassObject **ppNewInstance) {
	char funcname[64] = "SpawnInstance";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::CompareTo(long lFlags, IWbemClassObject *pCompareTo) {
	char funcname[64] = "CompareTo";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::GetPropertyOrigin(LPCWSTR wszName, BSTR *pstrClassName) {
	char funcname[64] = "GetPropertyOrigin";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::InheritsFrom(LPCWSTR strAncestor) {
	char funcname[64] = "InheritsFrom";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::GetMethod(LPCWSTR wszName, long lFlags, IWbemClassObject **ppInSignature, IWbemClassObject **ppOutSignature) {
	char funcname[64] = "GetMethod";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::PutMethod(LPCWSTR wszName, long lFlags, IWbemClassObject *pInSignature, IWbemClassObject *pOutSignature) {
	char funcname[64] = "PutMethod";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::DeleteMethod(LPCWSTR wszName) {
	char funcname[64] = "DeleteMethod";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::BeginMethodEnumeration(long lEnumFlags) {
	char funcname[64] = "BeginMethodEnumeration";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::NextMethod(long lFlags, BSTR *pstrName, IWbemClassObject **ppInSignature, IWbemClassObject **ppOutSignature) {
	char funcname[64] = "NextMethod";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::EndMethodEnumeration( void) {
	char funcname[64] = "EndMethodEnumeration";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::GetMethodQualifierSet(LPCWSTR wszMethod, IWbemQualifierSet **ppQualSet) {
	char funcname[64] = "GetMethodQualifierSet";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOProxyWbemClassObject::GetMethodOrigin(LPCWSTR wszMethodName, BSTR *pstrClassName) {
	char funcname[64] = "GetMethodOrigin";
	init_funcname_proxy(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}
