#include "wbem.hpp"

const char CeVIOUnknown::classname[] = "CeVIOUnknown";
const char CeVIOEnumWbemClassObject::classname[] = "CeVIOEnumWbemClassObject";
const char CeVIOWbemClassObject::classname[] = "CeVIOWbemClassObject";


void init_funcname(char *classname, char *funcname) {
	char newbuf[64];
	strcpy(newbuf, funcname);
	strcpy(funcname, classname);
	strcat(funcname, "::");
	strcat(funcname, newbuf);
	return;
}


CeVIOUnknown::CeVIOUnknown() {
	debug_info(this->classname, "Initialized.");
}

CeVIOEnumWbemClassObject::CeVIOEnumWbemClassObject() {
	debug_info(this->classname, "Initialized.");
}

CeVIOWbemClassObject::CeVIOWbemClassObject() {
	debug_info(this->classname, "Initialized.");
}


ULONG CeVIOUnknown::AddRef() {
	char funcname[64] = "AddRef";
	init_funcname(this->classname, funcname)

	this->refcount++;
	debug_info(funcname, "Called. refcount: %lu", this->refcount)
	return this->refcount;
}

HRESULT CeVIOUnknown::QueryInterface(REFIID riid, void **ppvObject) {
	char funcname[64] = "QueryInterface";
	init_funcname(this->classname, funcname);

	debug_info(funcname, "Called.");

	if (!ppvObject) return E_INVALIDARG;

	*ppvObject = NULL;

	if (riid == IID_IUnknown) {
		debug_info(funcname, "IID_IUnknown matched.");
		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	debug_info(funcname, "No matching REFIID found!");
	return E_NOINTERFACE;
}

ULONG CeVIOUnknown::Release() {
	char funcname[64] = "Release";
	init_funcname(this->classname, funcname);

	this->refcount--;
	debug_info(funcname, "Called. refcount: %lu", this->refcount);
	return this->refcount;
}


HRESULT CeVIOEnumWbemClassObject::Clone(IEnumWbemClassObject **ppEnum) {
	char funcname[64] = "Clone";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}
HRESULT CeVIOEnumWbemClassObject::Next(long lTimeout, ULONG uCount, IWbemClassObject **apObjects, ULONG *puReturned) {
	char funcname[64] = "Next";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOEnumWbemClassObject::NextAsync(ULONG uCount, IWbemObjectSink *pSink) {
	char funcname[64] = "NextAsync";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOEnumWbemClassObject::Reset() {
	char funcname[64] = "Reset";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOEnumWbemClassObject::Skip(long lTimeout, ULONG nCount) {
	char funcname[64] = "Skip";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}


HRESULT CeVIOWbemClassObject::GetQualifierSet(IWbemQualifierSet **ppQualSet) {
	char filename[64] = "GetQualifierSet";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::Get(LPCWSTR wszName, long lFlags, VARIANT *pVal, CIMTYPE *pType, long *plFlavor) {
	char filename[64] = "Get";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::Put(LPCWSTR wszName, long lFlags, VARIANT *pVal, CIMTYPE Type) {
	char filename[64] = "Put";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::Delete(LPCWSTR wszName) {
	char filename[64] = "Delete";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::GetNames(LPCWSTR wszQualifierName, long lFlags, VARIANT *pQualifierVal, SAFEARRAY * *pNames) {
	char filename[64] = "GetNames";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::BeginEnumeration(long lEnumFlags) {
	char filename[64] = "BeginEnumeration";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::Next(long lFlags, BSTR *strName, VARIANT *pVal, CIMTYPE *pType, long *plFlavor) {
	char filename[64] = "Next";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::EndEnumeration( void) {
	char filename[64] = "EndEnumeration";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::GetPropertyQualifierSet(LPCWSTR wszProperty, IWbemQualifierSet **ppQualSet) {
	char filename[64] = "GetPropertyQualifierSet";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::Clone(IWbemClassObject **ppCopy) {
	char filename[64] = "Clone";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::GetObjectText(long lFlags, BSTR *pstrObjectText) {
	char filename[64] = "GetObjectText";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::SpawnDerivedClass(long lFlags, IWbemClassObject **ppNewClass) {
	char filename[64] = "SpawnDerivedClass";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::SpawnInstance(long lFlags, IWbemClassObject **ppNewInstance) {
	char filename[64] = "SpawnInstance";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::CompareTo(long lFlags, IWbemClassObject *pCompareTo) {
	char filename[64] = "CompareTo";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::GetPropertyOrigin(LPCWSTR wszName, BSTR *pstrClassName) {
	char filename[64] = "GetPropertyOrigin";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::InheritsFrom(LPCWSTR strAncestor) {
	char filename[64] = "InheritsFrom";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::GetMethod(LPCWSTR wszName, long lFlags, IWbemClassObject **ppInSignature, IWbemClassObject **ppOutSignature) {
	char filename[64] = "GetMethod";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::PutMethod(LPCWSTR wszName, long lFlags, IWbemClassObject *pInSignature, IWbemClassObject *pOutSignature) {
	char filename[64] = "PutMethod";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::DeleteMethod(LPCWSTR wszName) {
	char filename[64] = "DeleteMethod";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::BeginMethodEnumeration(long lEnumFlags) {
	char filename[64] = "BeginMethodEnumeration";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::NextMethod(long lFlags, BSTR *pstrName, IWbemClassObject **ppInSignature, IWbemClassObject **ppOutSignature) {
	char filename[64] = "NextMethod";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::EndMethodEnumeration( void) {
	char filename[64] = "EndMethodEnumeration";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::GetMethodQualifierSet(LPCWSTR wszMethod, IWbemQualifierSet **ppQualSet) {
	char filename[64] = "GetMethodQualifierSet";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}

HRESULT CeVIOWbemClassObject::GetMethodOrigin(LPCWSTR wszMethodName, BSTR *pstrClassName) {
	char filename[64] = "GetMethodOrigin";
	init_funcname(this->classname, funcname);

	debug_error(funcname, "Not Implemented.");
	return E_NOTIMPL;
}
