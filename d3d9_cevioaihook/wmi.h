#pragma once
#ifndef INCL_WMI_H
#define INCL_WMI_H

#include <Windows.h>
#include <wbemcli.h>
#include "llist.h"
#include "util.h"

typedef struct comenumtype {
	CIMTYPE type;
	void* data;
} COMEnumType;

typedef struct wbemclasstype {
	const wchar_t* relpath;
	Linkedlist* com_llist;
} WbemClassType;


class HookWbemClassObject : public IWbemClassObject {
public:
	HookWbemClassObject(Linkedlist* llist, const wchar_t* relpath);


	ULONG AddRef();

	HRESULT QueryInterface(REFIID riid, void** ppvObject);

	ULONG Release();


	HRESULT BeginEnumeration(long lEnumFlags);

	HRESULT BeginMethodEnumeration(long lEnumFlags);

	HRESULT Clone(IWbemClassObject** ppCopy);

	HRESULT CompareTo(long lFlags, IWbemClassObject* pCompareTo);

	HRESULT Delete(LPCWSTR wszName);

	HRESULT DeleteMethod(LPCWSTR wszName);

	HRESULT EndEnumeration(void);

	HRESULT EndMethodEnumeration(void);

	HRESULT Get(LPCWSTR wszName, long lFlags, VARIANT* pVal, CIMTYPE* pType, long* plFlavor);

	HRESULT GetMethod(LPCWSTR wszName, long lFlags, IWbemClassObject** ppInSignature, IWbemClassObject** ppOutSignature);

	HRESULT GetMethodOrigin(LPCWSTR wszMethodName, BSTR* pstrClassName);

	HRESULT GetMethodQualifierSet(LPCWSTR wszMethod, IWbemQualifierSet** ppQualSet);

	HRESULT GetNames(LPCWSTR wszQualifierName, long lFlags, VARIANT* pQualifierVal, SAFEARRAY** pNames);

	HRESULT GetObjectText(long lFlags, BSTR* pstrObjectText);

	HRESULT GetPropertyOrigin(LPCWSTR wszName, BSTR* pstrClassName);

	HRESULT GetPropertyQualifierSet(LPCWSTR wszProperty, IWbemQualifierSet** ppQualSet);

	HRESULT GetQualifierSet(IWbemQualifierSet** ppQualSet);

	HRESULT InheritsFrom(LPCWSTR strAncestor);

	HRESULT Next(long lFlags, BSTR* strName, VARIANT* pVal, CIMTYPE* pType, long* plFlavor);

	HRESULT NextMethod(long lFlags, BSTR* pstrName, IWbemClassObject** ppInSignature, IWbemClassObject** ppOutSignature);

	HRESULT Put(LPCWSTR wszName, long lFlags, VARIANT* pVal, CIMTYPE Type);

	HRESULT PutMethod(LPCWSTR wszName, long lFlags, IWbemClassObject* pInSignature, IWbemClassObject* pOutSignature);

	HRESULT SpawnDerivedClass(long lFlags, IWbemClassObject** ppNewClass);

	HRESULT SpawnInstance(long lFlags, IWbemClassObject** ppNewInstance);

protected:
	Linkedlist* llist;
	ULONG refcount = 1;
	const wchar_t* relpath = NULL;
};

class HookEnumWbemClassObject : public IEnumWbemClassObject {
public:
	HookEnumWbemClassObject(Linkedlist* llist, const wchar_t* relpath);

	HookEnumWbemClassObject(Linkedlist** llist_list, size_t len, const wchar_t* relpath);

	HookEnumWbemClassObject(Linkedlist** llist_list, size_t len, const wchar_t* relpath, ULONG index);


	ULONG AddRef();

	HRESULT QueryInterface(REFIID riid, void** ppvObject);

	ULONG Release();


	HRESULT Clone(IEnumWbemClassObject** ppEnum);

	HRESULT Next(long lTimeout, ULONG uCount, IWbemClassObject** apObjects, ULONG* puReturned);

	HRESULT NextAsync(ULONG uCount, IWbemObjectSink* pSink);

	HRESULT Reset();

	HRESULT Skip(long lTimeout, ULONG nCount);

protected:
	ULONG refcount = 1;
	ULONG index = 0;
	void* buffer = NULL;
	Linkedlist** llist_list;
	size_t llist_len = 0;
	const wchar_t* relpath = NULL;
};

#endif
