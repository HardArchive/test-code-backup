// FRAMESAMPLEView.h : interface of the CFRAMESAMPLEView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAMESAMPLEVIEW_H__B066276E_E854_4111_85FA_0563DF5A1D41__INCLUDED_)
#define AFX_FRAMESAMPLEVIEW_H__B066276E_E854_4111_85FA_0563DF5A1D41__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\DROPFILEHANDLER.H"

class CFRAMESAMPLEView : public CWindowImpl<CFRAMESAMPLEView, CListBox>,
				 // Add CDropFilesHandler to your inheritance list...
				 public CDropFilesHandler<CFRAMESAMPLEView> 

{
public:
	DECLARE_WND_SUPERCLASS(NULL, CListBox::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CFRAMESAMPLEView)
		// Send WM_DROPFILES to its handler...
		CHAIN_MSG_MAP(CDropFilesHandler<CFRAMESAMPLEView>)
	END_MSG_MAP()


/////////////////////// FROM HERE DOWN, CODE IS NOT WIZARD-GENERATED. ///////////////
// CDropFilesHandler requisites.
	BOOL IsReadyForDrop(void) 	{ /*ResetContent(); */ return TRUE; }
	BOOL HandleDroppedFile(LPCTSTR szBuff)
	{
		ATLTRACE("%s\n", szBuff);
		// In this particular example, we'll do the VERY LEAST possible.
		AddString(szBuff); // You could also OPEN the file, if relevant...

	    // Return TRUE unless you're done handling files (e.g., if you want 
		// to handle only the first relevant file, and you have already found it).
		return TRUE;
	}
	void EndDropFiles(void)
	{
		// Sometimes, if your file handling is not trivial,  you might want to add all
		// file names to some container (std::list<CString> comes to mind), and do the 
		// handling afterwards, in a worker thread. 
		// If so, use this function to create your worker thread.
		ATLTRACE("Count of files in the last drop: %d\n", GetCount());
		// If you like, you can set the main frame's status bar to the count of files.
	}

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMESAMPLEVIEW_H__B066276E_E854_4111_85FA_0563DF5A1D41__INCLUDED_)
