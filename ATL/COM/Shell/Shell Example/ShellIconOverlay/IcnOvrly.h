/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.

   Copyright 1999 - 2000 Microsoft Corporation.  All Rights Reserved.
**************************************************************************/

/**************************************************************************

   File:          IcnOvrly.h

   Description:   

**************************************************************************/

/**************************************************************************
   #include statements
**************************************************************************/

#include <windows.h>
#include <initguid.h>
#include <shlobj.h>

/**************************************************************************
   function prototypes
**************************************************************************/

/**************************************************************************
   global variables and definitions
**************************************************************************/

// {EA3775F2-28BE-11d3-9C8D-00105A24ED29}
DEFINE_GUID(CLSID_SampleShellIconOverlay, 
0xea3775f2, 0x28be, 0x11d3, 0x9c, 0x8d, 0x0, 0x10, 0x5a, 0x24, 0xed, 0x29);

/**************************************************************************

   CClassFactory class definition

**************************************************************************/

class CClassFactory : public IClassFactory
{
protected:
   DWORD m_ObjRefCount;

public:
   CClassFactory();
   ~CClassFactory();

   //IUnknown methods
   STDMETHODIMP QueryInterface(REFIID, LPVOID FAR *);
   STDMETHODIMP_(DWORD) AddRef();
   STDMETHODIMP_(DWORD) Release();

   //IClassFactory methods
   STDMETHODIMP CreateInstance(LPUNKNOWN, REFIID, LPVOID FAR *);
   STDMETHODIMP LockServer(BOOL);
};

/**************************************************************************

   CShellIconOverlayID class definition

**************************************************************************/

class CShellIconOverlayID : public IShellIconOverlayIdentifier
{
protected:
   DWORD          m_ObjRefCount;
   
public:
   CShellIconOverlayID();
   ~CShellIconOverlayID();
   
   //IUnknown methods
   STDMETHODIMP QueryInterface(REFIID, LPVOID FAR *);
   STDMETHODIMP_(DWORD) AddRef();
   STDMETHODIMP_(DWORD) Release();

   //IShellIconOverlayIdentifier methods
   STDMETHODIMP IsMemberOf(LPCWSTR pwszPath, DWORD dwAttr);
   STDMETHODIMP GetOverlayInfo(LPWSTR pwszIconFile, int cchMax, LPINT pIndex, LPDWORD pdwFlags);
   STDMETHODIMP GetPriority(LPINT pPriority);
};

#ifdef _cplusplus
extern "C" {
#endif   //_cplusplus

BOOL DeleteEntireKey(HKEY hKey, LPTSTR pszSubKey);
int WideCharToLocal(LPTSTR pLocal, LPCWSTR pWide, DWORD dwChars);
int LocalToWideChar(LPWSTR pWide, LPCTSTR pLocal, DWORD dwChars);

#ifdef _cplusplus
}
#endif   //_cplusplus

#define ARRAYSIZE(a)    (sizeof(a)/sizeof(a[0]))

