
//ServerDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "../../Common/Src/socket/IocpServer.h"
#include "../Global/helper.h"

//CServerDlg dialog
class CServerDlg : public CDialogEx, public IServerSocketListener
{
//Construction
public:
	CServerDlg(CWnd* pParent = NULL);	//standard constructor

//Dialog Data
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	//DDX/DDV support


//Implementation
protected:
	HICON m_hIcon;

	//Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg LRESULT CServerDlg::OnUserInfoMsg(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
public:
	void SetAppState(EnAppState state);
private:
	virtual ISocketListener::EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnClose(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult OnAccept(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnServerShutdown();

private:
	CListBox m_Info;
	CButton m_Start;
	CButton m_Stop;
	CIocpServer m_Server;
	EnAppState m_enState;

private:
	static const USHORT PORT;
	static const LPCTSTR ADDRESS;
};
