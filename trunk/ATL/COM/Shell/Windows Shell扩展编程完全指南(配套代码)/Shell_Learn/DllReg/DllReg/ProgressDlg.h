// ProgressDlg.h : CProgressDlg 的声明

#pragma once

#include "resource.h"       // 主符号

#include <atlhost.h>
#include <shlobj.h>

// CProgressDlg

class CProgressDlg : 
	public CAxDialogImpl<CProgressDlg>
{
public:
	CProgressDlg ( string_list* pFileList, CMINVOKECOMMANDINFO* pCmdInfo ) :
	  m_bStopSign(false), m_hList(NULL), m_pFileList(pFileList),
		  m_pCmdInfo(pCmdInfo)
	  { }

	  ~CProgressDlg()
	  {
	  }

	  enum { IDD = IDD_PROGRESSDLG };

	  BEGIN_MSG_MAP(CProgressDlg)
		  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		  NOTIFY_HANDLER(IDC_LIST, LVN_ITEMCHANGED, OnItemchangedList)
		  COMMAND_ID_HANDLER(IDC_STOP, OnStop)
		  COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		  CHAIN_MSG_MAP(CAxDialogImpl<CProgressDlg>)
	  END_MSG_MAP()

	  // 处理程序原型:
	  //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	  //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	  LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	  LRESULT OnItemchangedList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	  LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	  {
		  m_bStopSign = true;
		  return 0;
	  }

	  LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	  {
		  EndDialog(wID);
		  return 0;
	  }

protected:
	bool         m_bStopSign;
	HWND         m_hList;
	string_list* m_pFileList;
	string_list  m_lsStatusMessages;    // list of status messages for the dialog
	CMINVOKECOMMANDINFO* m_pCmdInfo;

	void DoWork();
};


