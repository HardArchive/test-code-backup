
// ServerDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "../../../Common/Src/socket/HPSocket.h"
#include "../../Global/helper.h"

// CServerDlg dialog
class CServerDlg : public CDialogEx, public CTcpPullServerListener
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg LRESULT CServerDlg::OnUserInfoMsg(WPARAM wp, LPARAM lp);
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnEnChangeConnId();
	DECLARE_MESSAGE_MAP()
public:
	void SetAppState(EnAppState state);
private:
	virtual EnHandleResult OnPrepareListen(SOCKET soListen);
	virtual ISocketListener::EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient);
	virtual ISocketListener::EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnReceive(CONNID dwConnID, int iLength);
	virtual ISocketListener::EnHandleResult OnClose(CONNID dwConnID);
	virtual ISocketListener::EnHandleResult OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult OnServerShutdown();

private:
	TPkgInfo* FindPkgInfo(CONNID dwConnID);
	void RemovePkgInfo(CONNID dwConnID);

private:
	CListBox m_Info;
	CButton m_Start;
	CButton m_Stop;
	CEdit m_Address;
	CEdit m_ConnID;
	CButton m_DisConn;

private:
	static const USHORT PORT;
	static const LPCTSTR ADDRESS;


	EnAppState m_enState;
	CString m_strAddress;

	//hash_map<CONNID, TPkgInfo*> m_mpPkgInfo;
	
	//CCriSec m_csPkgInfo;

	CTcpPullServerWrapper m_Server;
};
