// IPCWorkshopDlg.h : header file
//

#if !defined(AFX_IPCWORKSHOPDLG_H__F268EB71_EF8D_4A01_B9BC_1133D569A503__INCLUDED_)
#define AFX_IPCWORKSHOPDLG_H__F268EB71_EF8D_4A01_B9BC_1133D569A503__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CIPCWorkshopDlg dialog

// Contains definitions of Inter Process Communication mechanisms.
#include "InterProcessComm\Clipboard.h"
#include "InterProcessComm\FileMapping.h"
#include "InterProcessComm\NamedPipe.h"
#include "InterProcessComm\Mailslot.h"
#include "InterProcessComm\DerivedSocket.h"


// This is a sample structure whose pointer is going to be passed across processes.
struct EmployeeData
{
	int m_nEmployeeNo;
	char m_strName[50];
	char m_strAddress[100];
	char m_strDateOfBirth[50];
	char m_strPhone[50];
};


class CIPCWorkshopDlg : public CDialog
{
// Construction
public:
	CIPCWorkshopDlg(CWnd* pParent = NULL);	// standard constructor
	void Refresh();
	void InitializeServer();
	void UpdateUI(bool);
	void SetVariables(char*);
	CString GetSelectedIPC();
// Dialog Data
	//{{AFX_DATA(CIPCWorkshopDlg)
	enum { IDD = IDD_IPCWORKSHOP_DIALOG };
	CButton	m_ctrlProcess;
	CComboBox	m_ctrlIPCMechanism;
	CString	m_strName;
	CString	m_strPhone;
	CString	m_strDOB;
	int		m_nEmpId;
	CString	m_strAddress;
	CString	m_strServerName;
	CString	m_strIPAddress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCWorkshopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
	LRESULT OnMessageTransfer( WPARAM wparam, LPARAM lparam );

// Implementation
protected:
	HICON m_hIcon;
	static UINT m_nTransferMessage;
	bool m_nIsServer;
	
	// Communication Mediums
	CClipboard m_objClipboard;
	CFileMapping m_objFileMapping;
	CNamedPipe m_objNamedPipe;
	CMailslot m_objMailslot;
	CDerivedSocket m_objSocket;			// CSocket is more efficient.

	// Generated message map functions
	//{{AFX_MSG(CIPCWorkshopDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadioServer();
	afx_msg void OnRadioClient();
	afx_msg void OnSend();
	afx_msg void OnRadioLocal();
	afx_msg void OnRadioIntranet();
	afx_msg void OnRadioInternet();
	afx_msg void OnConnect();
	afx_msg void OnSelchangeComboIpcmechanisms();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCWORKSHOPDLG_H__F268EB71_EF8D_4A01_B9BC_1133D569A503__INCLUDED_)
