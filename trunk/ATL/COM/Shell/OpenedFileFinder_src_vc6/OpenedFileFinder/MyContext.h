#include <Windows.h>
#include <winbase.h>

// This class is used to make my application use comctl6 so that it have the xp styles. For more information about this
// check the following link 
// http://groups.google.com/group/microsoft.public.platformsdk.shell/browse_thread/thread/f1aece47175ebae8/17d14d09ecf4ecfd?lnk=st&rnum=7#17d14d09ecf4ecfd
class CMyContext 
{ 
	public: 
		BOOL Init() 
		{ 
			BOOL bRet = FALSE; 
			BOOL bTemp = FALSE;
			OSVERSIONINFO info = { 0 }; 
			info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
            bTemp = GetVersionEx(&info); 
            if (bTemp) 
            { 
                  //do special XP theme activation code only on XP or higher... 
              if ( (info.dwMajorVersion >= 5) && 
                   (info.dwMinorVersion >= 1) && 
                   (info.dwPlatformId == VER_PLATFORM_WIN32_NT)) 
               { 
                    ACTCTX actctx = {0}; 
					TCHAR szModule[MAX_PATH] = {0}; 
					HINSTANCE hinst = AfxGetInstanceHandle(); 
					::GetModuleFileName(hinst, szModule, MAX_PATH); 
				   actctx.cbSize = sizeof(ACTCTX); 
				   actctx.dwFlags = ACTCTX_FLAG_HMODULE_VALID | 
									ACTCTX_FLAG_RESOURCE_NAME_VALID; 
				   actctx.lpSource = szModule; 
				   actctx.lpResourceName = MAKEINTRESOURCE(2); 
				   actctx.hModule = hinst; 
				   m_hActCtx = ::CreateActCtx(&actctx); 
				   if (INVALID_HANDLE_VALUE != m_hActCtx) 
                   { 
	                  bRet = TRUE; 
                   } 
				} 
            } 
			return bRet; 
		} 


      CMyContext() : m_ulActivationCookie(0), m_hActCtx(0) 
      { 
            BOOL bRet = Init(); 
            if (bRet && m_hActCtx && (INVALID_HANDLE_VALUE != m_hActCtx)) 
            ActivateActCtx(m_hActCtx, &m_ulActivationCookie); 
      } 


      ~CMyContext() 
      { 
             if (m_hActCtx && (m_hActCtx!= INVALID_HANDLE_VALUE)) 
             { 
                    DeactivateActCtx(0, m_ulActivationCookie); 
                    ReleaseActCtx(m_hActCtx); 
             } 
      } 
  
  private: 

      unsigned long m_ulActivationCookie; 
      HANDLE m_hActCtx; 

};