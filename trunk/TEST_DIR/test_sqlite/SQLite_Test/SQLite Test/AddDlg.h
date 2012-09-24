#pragma once
#include "afxwin.h"
#include "CarsTbl.h"


// CAddDlg dialog

class CAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddDlg)

public:
    CAddDlg(CWnd* pParent = NULL);

    bool MyDoModal(CCarsTbl* pCars);

// Dialog Data
	enum { IDD = IDD_ADD };

protected:
    CCarsTbl* m_pCars;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
