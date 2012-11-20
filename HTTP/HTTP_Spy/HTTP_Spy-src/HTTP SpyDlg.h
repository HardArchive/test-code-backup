// HTTP SpyDlg.h : header file
//

#if !defined(AFX_HTTPSPYDLG_H__16DDAA76_5AD9_4A66_B351_69AADF726642__INCLUDED_)
#define AFX_HTTPSPYDLG_H__16DDAA76_5AD9_4A66_B351_69AADF726642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MyTabCtrl.h"
#include "HttpSocket.h"
/////////////////////////////////////////////////////////////////////////////
// CHTTPSpyDlg dialog

class CHTTPSpyDlg : public CDialog
{
// Construction
	enum RESET_TYPE resetType;
	HttpParams *m_httpParams;

public:
	CHTTPSpyDlg(CWnd* pParent = NULL);	// standard constructor
    static BOOL SysCommandKey;
	static CString WindowTitle;
	CString strUserAgent;
    enum{ cFirstIndex=1};
    long CHTTPSpyDlg::ScanRadioGroup(BOOL bGetIndex,int nIndex=cFirstIndex-1)const;
	
	//Helper methods to set/get radios bounded by the groupbox.
	bool		IsValidControl(CWnd* pWnd)const
	{return pWnd!=NULL && pWnd->GetParent()==m_Group_Request.GetParent();}

	HTTP_METHOD getMethod(void) { return m_httpParams->getHttpMethod(); }
	
	void setMethod(HTTP_METHOD httpMethod)
	{
		this->m_httpParams->setHttpMethod(httpMethod);
	}
	
	CONN_TYPE getConnType(void) { return m_httpParams->getConnType(); }
	
	void setConnType(CONN_TYPE connType) 
	{ 
		this->m_httpParams->setConnType(connType);
	}

	RESET_TYPE getResetType(void) { return resetType; }

	void setResetType(RESET_TYPE resType) { this->resetType = resType; }

	static UINT doHttpProcessing(LPVOID);

	//Sets the active radio by index. The index is 1 based and the order  
	//is in the order ehey are created in the resource definition.
	void		SetRadio(int nRadio){ScanRadioGroup(FALSE,nRadio);}	
	//Return the 1 based index of the active radio.	
	int			GetRadio()const			{return ScanRadioGroup(TRUE);}

	static bool  IsAutoRadio(CWnd* pButton)
	{
		int Style = 0;
		if(pButton!=NULL)
		      Style = (UINT)::GetWindowLong(pButton->GetSafeHwnd(),GWL_STYLE);
		return (Style & BS_AUTORADIOBUTTON)!=0;
	}

	BOOL resetDialog(void);

// Dialog Data
	//{{AFX_DATA(CHTTPSpyDlg)
	enum { IDD = IDD_HTTPSPY_DIALOG };
	CComboBox	m_comboAgent;
	CButton	m_Group_Request;
	CMyTabCtrl	m_TabCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTTPSpyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHTTPSpyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeEditPort();
	afx_msg void OnChangeEditUrl();
	afx_msg void OnMethodHead();
    afx_msg void OnMethodGet();
	afx_msg void OnMethodDoIt();
	afx_msg void OnSelchangeComboUsragent();
	afx_msg void OnButtonReset();
	afx_msg void OnResetSetting();
	afx_msg void OnResetResult();
	afx_msg void OnDirectInternet();
	afx_msg void OnHttpProxy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPSPYDLG_H__16DDAA76_5AD9_4A66_B351_69AADF726642__INCLUDED_)
