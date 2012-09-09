// MyOverlayIcon.cpp : Implementation of 

#include "stdafx.h"
#include "MyOverlayIcon.h"


// CMyOverlayIcon

// IShellIconOverlayIdentifier Method Implementation 
// IShellIconOverlayIdentifier::GetOverlayInfo
// returns The Overlay Icon Location to the system
STDMETHODIMP CMyOverlayIcon::GetOverlayInfo(
  LPWSTR pwszIconFile,
  int cchMax,int* pIndex,
  DWORD* pdwFlags)
{
  GetModuleFileNameW(_AtlBaseModule.GetModuleInstance(), pwszIconFile, cchMax);

  *pIndex = 0;
  *pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;

  return S_OK;
}

// IShellIconOverlayIdentifier Method Implementation 

// returns the priority of this overlay 0 being the highest. 
// this overlay is always selected do to its high priority 
STDMETHODIMP CMyOverlayIcon::GetPriority(int* pPriority)
{
  // highest priority
  *pPriority=0;
  return S_OK;
}

// IShellIconOverlayIdentifier Method Implementation
// IShellIconOverlayIdentifier::IsMemberOf
// Returns Whether the object should have this overlay or not 
STDMETHODIMP CMyOverlayIcon::IsMemberOf(LPCWSTR pwszPath, DWORD dwAttrib)
{
  wchar_t *s = _wcsdup(pwszPath);
  HRESULT r = S_FALSE;
  
  _wcslwr(s);

  // Criteria
  if (wcsstr(s, L"codeproject") != 0)
    r = S_OK;

  free(s);

  return r;
}
