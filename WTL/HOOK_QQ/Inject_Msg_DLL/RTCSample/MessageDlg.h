#if !defined(AFX_MESSAGEDLG_H__1EA476E6_5482_4D70_8783_90FF68A5A0B3__INCLUDED_)
#define AFX_MESSAGEDLG_H__1EA476E6_5482_4D70_8783_90FF68A5A0B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg dialog

class CMessageDlg : public CDialog
{
// Construction
public:
	void SetSession ( IRTCSession *pSession );
	void SetParentClient ( IRTCClient *pClient );
	IRTCClient * m_pParentClient;
	HRESULT DeliverUserStatus(IRTCParticipant * pParticipant, RTC_MESSAGING_USER_STATUS enStatus);
	HRESULT DeliverMessage(IRTCParticipant * pParticipant, BSTR bstrContentType, BSTR bstrMessage);
	CMessageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMessageDlg)
	enum { IDD = IDD_MESSAGEDLG };
	CListBox	m_cMessage;
	CEdit	m_cText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageDlg)
	afx_msg void OnSendtext();
	afx_msg void OnClear();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DoDisplayMessage();
	IRTCSession * m_pParentSession;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEDLG_H__1EA476E6_5482_4D70_8783_90FF68A5A0B3__INCLUDED_)
