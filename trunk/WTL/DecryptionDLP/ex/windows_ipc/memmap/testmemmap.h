#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "memmap.h"


class CTestmemmapApp : public CWinApp
{
public:
	CTestmemmapApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


class CDialog1 : public CDialog
{
public:
	CDialog1(CWnd* pParent = NULL); //standard constructor

protected:
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_ctrlEdit;
  CMemMapFile m_mmf;
  UINT_PTR m_nTimer;

	virtual void DoDataExchange(CDataExchange* pDX); //DDX/DDV support
	virtual BOOL OnInitDialog();
	
	afx_msg void OnChangeEdit1();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};
