/**************************************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' WITHOUT WARRANTY OF
   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
   PARTICULAR PURPOSE.

   Copyright 1999 - 2000 Microsoft Corporation.  All Rights Reserved.
**************************************************************************/

/**************************************************************************

   File:          IcnOvrly.cpp

   Description:   

**************************************************************************/

/**************************************************************************
   #include statements
**************************************************************************/

#include "IcnOvrly.h"
#include <olectl.h>
#include <shlguid.h>
#include <shlwapi.h>

/**************************************************************************
   private function prototypes
**************************************************************************/

extern "C" BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID);

/**************************************************************************
   global variables
**************************************************************************/

HINSTANCE   g_hInst;
DWORD       g_DllRefCount;

#define STR_EXT_TITLE   TEXT("Sample Shell Icon Overlay Identifier")
#define STR_REG_PATH    TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ShellIconOverlayIdentifiers")

/**************************************************************************

   DllMain

**************************************************************************/

extern "C" BOOL WINAPI DllMain(  HINSTANCE hInstance, 
                                 DWORD dwReason, 
                                 LPVOID lpReserved)
{
switch(dwReason)
   {
   case DLL_PROCESS_ATTACH:
      g_hInst = hInstance;
      break;

   case DLL_PROCESS_DETACH:
      //MessageBeep(MB_OK);
      break;
   }
   
return TRUE;
}                                 

/**************************************************************************

   DllCanUnloadNow

**************************************************************************/

STDAPI DllCanUnloadNow(void)
{
return ((g_DllRefCount == 0) ? S_OK : S_FALSE);
}

/**************************************************************************

   DllGetClassObject

**************************************************************************/

STDAPI DllGetClassObject(  REFCLSID rclsid, 
                           REFIID riid, 
                           LPVOID *ppReturn)
{
*ppReturn = NULL;

//if we don't support this classid, return the proper error code
if(!IsEqualCLSID(rclsid, CLSID_SampleShellIconOverlay))
   return CLASS_E_CLASSNOTAVAILABLE;
   
//create a CClassFactory object and check it for validity
CClassFactory *pClassFactory = new CClassFactory();
if(NULL == pClassFactory)
   return E_OUTOFMEMORY;
   
//get the QueryInterface return for our return value
HRESULT hResult = pClassFactory->QueryInterface(riid, ppReturn);

//call Release to decement the ref count - creating the object set it to one 
//and QueryInterface incremented it - since its being used externally (not by 
//us), we only want the ref count to be 1
pClassFactory->Release();

//return the result from QueryInterface
return hResult;
}

///////////////////////////////////////////////////////////////////////////
//
// CClassFactory member function
//

/**************************************************************************

   CClassFactory::CClassFactory

**************************************************************************/

CClassFactory::CClassFactory()
{
m_ObjRefCount = 1;
g_DllRefCount++;
}

/**************************************************************************

   CClassFactory::~CClassFactory

**************************************************************************/

CClassFactory::~CClassFactory()
{
g_DllRefCount--;
}

/**************************************************************************

   CClassFactory::QueryInterface

**************************************************************************/

STDMETHODIMP CClassFactory::QueryInterface(   REFIID riid, 
                                             LPVOID FAR * ppReturn)
{
if(IsEqualIID(riid, IID_IUnknown))
   {
   *ppReturn = (LPUNKNOWN)(LPCLASSFACTORY)this;
   m_ObjRefCount++;
   return S_OK;
   }
   
if(IsEqualIID(riid, IID_IClassFactory))
   {
   *ppReturn = (LPCLASSFACTORY)this;
   m_ObjRefCount++;
   return S_OK;
   }   

*ppReturn = NULL;
return E_NOINTERFACE;
}                                             

/**************************************************************************

   CClassFactory::AddRef

**************************************************************************/

STDMETHODIMP_(DWORD) CClassFactory::AddRef()
{
return ++m_ObjRefCount;
}


/**************************************************************************

   CClassFactory::Release

**************************************************************************/

STDMETHODIMP_(DWORD) CClassFactory::Release()
{
if(--m_ObjRefCount ==0)
   delete this;
   
return m_ObjRefCount;
}

/**************************************************************************

   CClassFactory::CreateInstance

**************************************************************************/

STDMETHODIMP CClassFactory::CreateInstance(  LPUNKNOWN pUnknown, 
                                             REFIID riid, 
                                             LPVOID FAR * ppObject)
{
*ppObject = NULL;

if(pUnknown != NULL)
   return CLASS_E_NOAGGREGATION;

//add implementation specific code here
CShellIconOverlayID *psioid = new CShellIconOverlayID;
if(NULL == psioid)
   return E_OUTOFMEMORY;
  
//get the QueryInterface return for our return value
HRESULT hResult = psioid->QueryInterface(riid, ppObject);

//call Release to decement the ref count
psioid->Release();

//return the result from QueryInterface
return hResult;

}

/**************************************************************************

   CClassFactory::LockServer

**************************************************************************/

STDMETHODIMP CClassFactory::LockServer(BOOL)
{
return E_NOTIMPL;
}

/**************************************************************************

   CShellIconOverlayID::CShellIconOverlayID()

**************************************************************************/

CShellIconOverlayID::CShellIconOverlayID()
{
m_ObjRefCount = 1;
g_DllRefCount++;
}

/**************************************************************************

   CShellIconOverlayID::~CShellIconOverlayID()

**************************************************************************/

CShellIconOverlayID::~CShellIconOverlayID()
{
g_DllRefCount--;
}

///////////////////////////////////////////////////////////////////////////
//
// IUnknown Implementation
//

/**************************************************************************

   CShellIconOverlayID::QueryInterface

**************************************************************************/

STDMETHODIMP CShellIconOverlayID::QueryInterface(  REFIID riid, 
                                                   LPVOID *ppReturn)
{
*ppReturn = NULL;

//IUnknown
if(IsEqualIID(riid, IID_IUnknown))
   {
   *ppReturn = this;
   }

//IContextMenu
else if(IsEqualIID(riid, IID_IShellIconOverlayIdentifier))
   {
   *ppReturn = (IShellIconOverlayIdentifier*)this;
   }

if(*ppReturn)
   {
   (*(LPUNKNOWN*)ppReturn)->AddRef();
   return S_OK;
   }

return E_NOINTERFACE;
}                                             

/**************************************************************************

   CShellIconOverlayID::AddRef

**************************************************************************/

STDMETHODIMP_(DWORD) CShellIconOverlayID::AddRef()
{
return ++m_ObjRefCount;
}


/**************************************************************************

   CShellIconOverlayID::Release

**************************************************************************/

STDMETHODIMP_(DWORD) CShellIconOverlayID::Release()
{
if(--m_ObjRefCount == 0)
   delete this;
   
return m_ObjRefCount;
}

///////////////////////////////////////////////////////////////////////////
//
// IShellIconOverlayIdentifier Implementation
//

/**************************************************************************

   CShellIconOverlayID::IsMemberOf()

   Called when a file or folder object is about to be displayed. Return 
   S_OK to use the overlay icon for this object or S_FALSE to not use the 
   overlay icon for this object.

**************************************************************************/

STDMETHODIMP CShellIconOverlayID::IsMemberOf(   LPCWSTR pwszPath, 
                                                DWORD dwAttr)
{
TCHAR szPath[MAX_PATH];
WideCharToLocal(szPath, pwszPath, MAX_PATH);

//display the overlay icon for files with the .TXT extension
LPTSTR   pszExt = PathFindExtension(szPath);
if(pszExt && (0 == lstrcmpi(pszExt, TEXT(".txt"))))
   return S_OK;

return S_FALSE;
}

/**************************************************************************

   CShellIconOverlayID::GetOverlayInfo()

   Called when the overlay icon identifier object is initialized. The object 
   will normally remain active for the life of the shell. This method is 
   called only once for each overlay identifier registered. This method 
   supplies the source file name and index, if required, of the overlay 
   icon for this object. If the icon source file only contains one icon, 
   it is not necessary to to add ISIOI_ICONINDEX to pdwFlags.

**************************************************************************/

STDMETHODIMP CShellIconOverlayID::GetOverlayInfo(  LPWSTR pwszIconFile, 
                                                   int cchMax, 
                                                   LPINT pIndex, 
                                                   LPDWORD pdwFlags)
{
if(IsBadWritePtr(pIndex, sizeof(int)))
   return E_INVALIDARG;

if(IsBadWritePtr(pdwFlags, sizeof(DWORD)))
   return E_INVALIDARG;

TCHAR szModule[MAX_PATH];

GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule));

LocalToWideChar(pwszIconFile, szModule, cchMax);

*pIndex = 0;

*pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;

return S_OK;
}

/**************************************************************************

   CShellIconOverlayID::GetPriority()

   Called to obtain the priority if there are conflicts between more than 
   one overlay icon. The shell can only display one overlay icon for an 
   object, so if more than one overlay should be displayed for an object, 
   then the priority is used to determine which overlay should be used.

**************************************************************************/

STDMETHODIMP CShellIconOverlayID::GetPriority(LPINT pPriority)
{
if(IsBadWritePtr(pPriority, sizeof(int)))
   return E_INVALIDARG;

*pPriority = 0;

return S_OK;
}

/**************************************************************************

   DllRegisterServer

**************************************************************************/

typedef struct{
   HKEY  hRootKey;
   LPTSTR lpszSubKey;
   LPTSTR lpszValueName;
   LPTSTR lpszData;
}REGSTRUCT, *LPREGSTRUCT;

STDAPI DllRegisterServer(void)
{
int      i;
HKEY     hKey;
LRESULT  lResult;
DWORD    dwDisp;
TCHAR    szSubKey[MAX_PATH];
TCHAR    szCLSID[MAX_PATH];
TCHAR    szModule[MAX_PATH];
LPWSTR   pwsz;

//get the CLSID in string form
StringFromIID(CLSID_SampleShellIconOverlay, &pwsz);

if(pwsz)
   {
   WideCharToLocal(szCLSID, pwsz, ARRAYSIZE(szCLSID));

   //free the string
   LPMALLOC pMalloc;
   CoGetMalloc(1, &pMalloc);
   if(pMalloc)
      {
      pMalloc->Free(pwsz);
      pMalloc->Release();
      }
   }

//get this DLL's path and file name
GetModuleFileName(g_hInst, szModule, ARRAYSIZE(szModule));

//register the CLSID entries
REGSTRUCT ClsidEntries[] = {  HKEY_CLASSES_ROOT,   TEXT("CLSID\\%s"),                                    NULL,                   STR_EXT_TITLE,
                              HKEY_CLASSES_ROOT,   TEXT("CLSID\\%s\\InprocServer32"),                    NULL,                   TEXT("%s"),
                              HKEY_CLASSES_ROOT,   TEXT("CLSID\\%s\\InprocServer32"),                    TEXT("ThreadingModel"), TEXT("Apartment"),
                              NULL,                NULL,                                                NULL,                   NULL};

for(i = 0; ClsidEntries[i].hRootKey; i++)
   {
   //Create the sub key string.
   wsprintf(szSubKey, ClsidEntries[i].lpszSubKey, szCLSID);

   lResult = RegCreateKeyEx(  ClsidEntries[i].hRootKey,
                              szSubKey,
                              0,
                              NULL,
                              REG_OPTION_NON_VOLATILE,
                              KEY_WRITE,
                              NULL,
                              &hKey,
                              &dwDisp);
   
   if(NOERROR == lResult)
      {
      TCHAR szData[MAX_PATH] = TEXT("");

      //if necessary, create the value string
      wsprintf(szData, ClsidEntries[i].lpszData, szModule);
   
      lResult = RegSetValueEx(   hKey,
                                 ClsidEntries[i].lpszValueName,
                                 0,
                                 REG_SZ,
                                 (LPBYTE)szData,
                                 lstrlen(szData) + 1);
      
      RegCloseKey(hKey);
      }
   else
      return SELFREG_E_CLASS;
   }

//register the shell icon overlay identifier
wsprintf(szSubKey, TEXT("%s\\%s"), STR_REG_PATH, STR_EXT_TITLE);
lResult = RegCreateKeyEx(  HKEY_LOCAL_MACHINE,
                           szSubKey,
                           0,
                           NULL,
                           REG_OPTION_NON_VOLATILE,
                           KEY_WRITE,
                           NULL,
                           &hKey,
                           &dwDisp);

if(NOERROR == lResult)
   {
   //Create the value string.
   lResult = RegSetValueEx(   hKey,
                              NULL,
                              0,
                              REG_SZ,
                              (LPBYTE)szCLSID,
                              (lstrlen(szCLSID) + 1) * sizeof(TCHAR));
   
   RegCloseKey(hKey);
   }
else
   return SELFREG_E_CLASS;

//If running on NT, register the extension as approved.
OSVERSIONINFO  osvi;

osvi.dwOSVersionInfoSize = sizeof(osvi);
GetVersionEx(&osvi);

if(VER_PLATFORM_WIN32_NT == osvi.dwPlatformId)
   {
   lstrcpyn( szSubKey, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"), ARRAYSIZE(szSubKey));

   lResult = RegCreateKeyEx(  HKEY_LOCAL_MACHINE,
                              szSubKey,
                              0,
                              NULL,
                              REG_OPTION_NON_VOLATILE,
                              KEY_WRITE,
                              NULL,
                              &hKey,
                              &dwDisp);

   if(NOERROR == lResult)
      {
      TCHAR szData[MAX_PATH];

      //Create the value string.
      lstrcpyn(szData, STR_EXT_TITLE, ARRAYSIZE(szData));

      lResult = RegSetValueEx(   hKey,
                                 szCLSID,
                                 0,
                                 REG_SZ,
                                 (LPBYTE)szData,
                                 (lstrlen(szData) + 1) * sizeof(TCHAR));
      
      RegCloseKey(hKey);
      }
   else
      return SELFREG_E_CLASS;
   }

return S_OK;
}

/**************************************************************************

   DllUnregisterServer()

**************************************************************************/

STDAPI DllUnregisterServer(VOID)
{
LPWSTR   pwsz;
TCHAR    szCLSID[MAX_PATH];
TCHAR    szSubKey[MAX_PATH];

//get the CLSID in string form
StringFromIID(CLSID_SampleShellIconOverlay, &pwsz);

if(pwsz)
   {
   WideCharToLocal(szCLSID, pwsz, ARRAYSIZE(szCLSID));

   //free the string
   LPMALLOC pMalloc;
   CoGetMalloc(1, &pMalloc);
   if(pMalloc)
      {
      pMalloc->Free(pwsz);
      pMalloc->Release();
      }
   }
else
   {
   return E_FAIL;
   }

//delete the namespace extension entries
wsprintf(szSubKey, TEXT("%s\\%s"), STR_REG_PATH, STR_EXT_TITLE);
DeleteEntireKey(HKEY_LOCAL_MACHINE, szSubKey);

//delete the object's registry entries
wsprintf(szSubKey, TEXT("CLSID\\%s"), szCLSID);
DeleteEntireKey(HKEY_CLASSES_ROOT, szSubKey);

//delete the approved extensions on NT
OSVERSIONINFO  osvi;

osvi.dwOSVersionInfoSize = sizeof(osvi);
GetVersionEx(&osvi);

if(VER_PLATFORM_WIN32_NT == osvi.dwPlatformId)
   {
   LRESULT  lResult;
   HKEY     hKey;

   lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                           TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                           0,
                           KEY_SET_VALUE,
                           &hKey);

   if(NOERROR == lResult)
      {
      lResult = RegDeleteValue(hKey, STR_EXT_TITLE);
      
      RegCloseKey(hKey);
      }
   else
      return SELFREG_E_CLASS;
   }

return S_OK;
}

/**************************************************************************

   DeleteEntireKey()

**************************************************************************/

BOOL DeleteEntireKey(HKEY hKey, LPTSTR pszSubKey)
{
LRESULT  lResult;
HKEY     hEnumKey;

lResult = RegOpenKeyEx( hKey,
                        pszSubKey,
                        0,
                        KEY_ENUMERATE_SUB_KEYS,
                        &hEnumKey);

if(NOERROR == lResult)
   {
   TCHAR szKey[MAX_PATH];
   DWORD dwSize = MAX_PATH;

   while(ERROR_SUCCESS == RegEnumKeyEx(hEnumKey, 0, szKey, &dwSize, NULL, NULL, NULL, NULL))
      {
      DeleteEntireKey(hEnumKey, szKey);

      dwSize = MAX_PATH;
      }
   
   RegCloseKey(hEnumKey);
   }
else
   {
   return FALSE;
   }

RegDeleteKey(hKey, pszSubKey);

return TRUE;
}

/**************************************************************************

   WideCharToLocal()
   
**************************************************************************/

int WideCharToLocal(LPTSTR pLocal, LPCWSTR pWide, DWORD dwChars)
{
*pLocal = 0;

#ifdef UNICODE
lstrcpyn(pLocal, pWide, dwChars);
#else
WideCharToMultiByte( CP_ACP, 
                     0, 
                     pWide, 
                     -1, 
                     pLocal, 
                     dwChars, 
                     NULL, 
                     NULL);
#endif

return lstrlen(pLocal);
}

/**************************************************************************

   LocalToWideChar()
   
**************************************************************************/

int LocalToWideChar(LPWSTR pWide, LPCTSTR pLocal, DWORD dwChars)
{
*pWide = 0;

#ifdef UNICODE
lstrcpyn(pWide, pLocal, dwChars);
#else
MultiByteToWideChar( CP_ACP, 
                     0, 
                     pLocal, 
                     -1, 
                     pWide, 
                     dwChars); 
#endif

return lstrlenW(pWide);
}

