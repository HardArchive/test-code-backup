
// ClientDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "../../Common/Src/socket/SocketClient.h"
#include "../Global/helper.h"


// CClientDlg dialog
class CClientDlg : public CDialogEx, public IClientSocketListener
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg LRESULT OnUserInfoMsg(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
public:
	void SetAppState(EnAppState state);
private:
	virtual ISocketListener::EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnClose(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult OnConnect(DWORD dwConnectionID);
private:
	BOOL CheckParams();
private:
	CComboBox m_TestTimes;
	CComboBox m_ThreadCount;
	CComboBox m_TestInterv;
	CComboBox m_ThreadInterv;
	CComboBox m_DataLen;
	CListBox m_Info;
	CEdit m_Address;
	CEdit m_Port;
	CButton m_Start;
	CButton m_Stop;

	CString m_strAddress;
	USHORT m_usPort;
	int m_iTestTimes;
	int m_iTestInterv;
	int m_iThreadCount;
	int m_iThreadInterv;
	int m_iDataLen;

	volatile LONGLONG m_llTotalReceived;
	volatile LONGLONG m_llTotalSent;
	LONGLONG m_llExpectReceived;
	DWORD m_dwBeginTickCount;
	DWORD m_dwEndTickCount;
	DWORD m_dwTimeconsuming;
	CBufferPtr m_sendBuffer;
	VectorWrapper<vector<CSocketClient*>, PtrSet_Cleaner<vector<CSocketClient*>>> m_vtClients;

	EnAppState m_enState;
};
