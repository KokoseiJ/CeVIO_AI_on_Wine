#ifndef INCL_COM_WBEM_PROXY_HPP
#define INCL_COM_WBEM_PROXY_HPP

#ifdef __MINGW32__
#include <initguid.h>
#else
#pragma comment(lib, "wbemuuid.lib")
#endif

#include "wbem_client.hpp"
extern "C" {
#include "../../debug.h"
}
#include <wbemcli.h>
#include <windows.h>
#include <inttypes.h>


class CeVIOProxyEnumWbemClassObject : public IEnumWbemClassObject {
private:
	const static char classname[];
	uint32_t refcount = 0;

	IEnumWbemClassObject *pEnum = NULL;

public:
	CeVIOProxyEnumWbemClassObject(IEnumWbemClassObject *pEnum);
	CeVIOProxyEnumWbemClassObject(BSTR queryStr, long flags);
	~CeVIOProxyEnumWbemClassObject();

	ULONG AddRef();
	HRESULT QueryInterface(REFIID riid, void **ppvObject);	
	ULONG Release();

	HRESULT Clone(IEnumWbemClassObject **ppEnum);
	HRESULT Next(long lTimeout, ULONG uCount, IWbemClassObject **apObjects, ULONG *puReturned);
	HRESULT NextAsync(ULONG uCount, IWbemObjectSink *pSink);
	HRESULT Reset();
	HRESULT Skip(long lTimeout, ULONG nCount);
};

class CeVIOProxyWbemClassObject : public IWbemClassObject {
private:
	const static char classname[];
	uint32_t refcount = 0;

	IWbemClassObject *pWbemClassObj = NULL;

public:
	CeVIOProxyWbemClassObject(IWbemClassObject *pWbemClassObj);
	~CeVIOProxyWbemClassObject();

	ULONG AddRef();
	HRESULT QueryInterface(REFIID riid, void **ppvObject);
	ULONG Release();

	HRESULT GetQualifierSet(IWbemQualifierSet **ppQualSet);
	HRESULT Get(LPCWSTR wszName, long lFlags, VARIANT *pVal, CIMTYPE *pType, long *plFlavor);
	HRESULT Put(LPCWSTR wszName, long lFlags, VARIANT *pVal, CIMTYPE Type);
	HRESULT Delete(LPCWSTR wszName);
	HRESULT GetNames(LPCWSTR wszQualifierName, long lFlags, VARIANT *pQualifierVal, SAFEARRAY * *pNames);
	HRESULT BeginEnumeration(long lEnumFlags);
	HRESULT Next(long lFlags, BSTR *strName, VARIANT *pVal, CIMTYPE *pType, long *plFlavor);
	HRESULT EndEnumeration( void);
	HRESULT GetPropertyQualifierSet(LPCWSTR wszProperty, IWbemQualifierSet **ppQualSet);
	HRESULT Clone(IWbemClassObject **ppCopy);
	HRESULT GetObjectText(long lFlags, BSTR *pstrObjectText);
	HRESULT SpawnDerivedClass(long lFlags, IWbemClassObject **ppNewClass);
	HRESULT SpawnInstance(long lFlags, IWbemClassObject **ppNewInstance);
	HRESULT CompareTo(long lFlags, IWbemClassObject *pCompareTo);
	HRESULT GetPropertyOrigin(LPCWSTR wszName, BSTR *pstrClassName);
	HRESULT InheritsFrom(LPCWSTR strAncestor);
	HRESULT GetMethod(LPCWSTR wszName, long lFlags, IWbemClassObject **ppInSignature, IWbemClassObject **ppOutSignature);
	HRESULT PutMethod(LPCWSTR wszName, long lFlags, IWbemClassObject *pInSignature, IWbemClassObject *pOutSignature);
	HRESULT DeleteMethod(LPCWSTR wszName);
	HRESULT BeginMethodEnumeration(long lEnumFlags);
	HRESULT NextMethod(long lFlags, BSTR *pstrName, IWbemClassObject **ppInSignature, IWbemClassObject **ppOutSignature);
	HRESULT EndMethodEnumeration( void);
	HRESULT GetMethodQualifierSet(LPCWSTR wszMethod, IWbemQualifierSet **ppQualSet);
	HRESULT GetMethodOrigin(LPCWSTR wszMethodName, BSTR *pstrClassName);
};


#endif
