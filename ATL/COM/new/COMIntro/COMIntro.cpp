// COMIntro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "COMIntro.h"
#include <atlconv.h>                    // ATL string conversion macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    // initialize MFC and print and error on failure
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
        {
        cerr << _T("Fatal Error: MFC initialization failed") << endl;
        return 1;
        }

    // Init the COM library - have Windows load up the DLLs.
    if ( FAILED( CoInitialize(NULL) ))
        {
        cerr << _T("Fatal Error: OLE initialization failed") << endl;
        return 1;
        }

WCHAR   wszWallpaper [MAX_PATH];
HRESULT hr;
IActiveDesktop* pIAD;

    // Create a COM object from the Active Desktop coclass.
    hr = CoCreateInstance ( CLSID_ActiveDesktop,
                            NULL,
                            CLSCTX_INPROC_SERVER,
                            IID_IActiveDesktop,
                            (void**) &pIAD );

    if ( SUCCEEDED(hr) )
        {
        // Get the name of the wallpaper file.
        hr = pIAD->GetWallpaper ( wszWallpaper, MAX_PATH, 0 );

        if ( SUCCEEDED(hr) )
            {
            wcout << L"Wallpaper path is:\n    " << wszWallpaper << endl << endl;
            }
        else
            {
            cout << _T("GetWallpaper() failed.") << endl << endl;
            }

        // Release the IActiveDesktop interface, since we're done using it.
        pIAD->Release();
        }
    else
        {
        cout << _T("CoInitialize() failed.") << endl << endl;
        }

    // If anything above failed, quit the program.
    if ( FAILED(hr) )
        return 0;

CString       sWallpaper = wszWallpaper;    // Convert the Unicode string to ANSI.
IShellLink*   pISL;
IPersistFile* pIPF;

    // Create a COM object from the Shell Link coclass.
    hr = CoCreateInstance ( CLSID_ShellLink,
                            NULL,
                            CLSCTX_INPROC_SERVER,
                            IID_IShellLink,
                            (void**) &pISL );

    if ( SUCCEEDED(hr) )
        {
        // Set the path of the target file (the wallpaper).
        hr = pISL->SetPath ( sWallpaper );

        if ( SUCCEEDED(hr) )
            {
            // Get an IPersisteFile interface from the COM object.
            hr = pISL->QueryInterface ( IID_IPersistFile, (void**) &pIPF );

            if ( SUCCEEDED(hr) )
                {
                // Save the shortcut as "C:\wallpaper.lnk"  Note that the first
                // param to IPersistFile::Save() is a Unicode string, thus the L
                // prefix.
                hr = pIPF->Save ( L"C:\\wallpaper.lnk", FALSE );

                if ( SUCCEEDED(hr) )
                    {
                    cout << _T("Shortcut created.") << endl << endl;
                    }
                else
                    {
                    cout << _T("Save() failed.") << endl << endl;
                    }

                // Release the IPersistFile interface, since we're done with it.
                pIPF->Release();
                }
            else
                {
                cout << _T("QueryInterface() failed.") << endl << endl;
                }
            }
        else
            {
            cout << _T("SetPath() failed.") << endl << endl;
            }

        // Release the IShellLink interface too.
        pISL->Release();
        }
    else
        {
        cout << _T("CoCreateInstance() failed.") << endl << endl;
        }

    CoUninitialize();

    return 0;
}
