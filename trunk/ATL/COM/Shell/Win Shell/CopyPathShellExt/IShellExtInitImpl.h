// IShellExtInitImpl.h

// This code was written by Dino Esposito and taken
// from his book, Visual C++ Windows Shell Programming

#include <AtlCom.h>
#include <ShlObj.h>

class ATL_NO_VTABLE IShellExtInitImpl : public IShellExtInit
{
public:

   // IUnknown
   STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) = 0;
   _ATL_DEBUG_ADDREF_RELEASE_IMPL(IShellExtInitImpl)

   // IShellExtInit
   STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY)
   {
      return S_FALSE;
   }
};
