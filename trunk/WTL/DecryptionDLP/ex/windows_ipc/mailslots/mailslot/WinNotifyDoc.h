#pragma once

#include "mailslot.h"

//Structure which will be sent down the mailslot
class CNotifyMessage
{
public:
//Enums
  enum 
  {
    MaxNameSize = MAX_COMPUTERNAME_LENGTH,
    MaxMessageSize = 100
  };

//Member variables
  SYSTEMTIME m_SentTime;
  TCHAR m_sSenderName[MaxNameSize + 1];
  TCHAR m_sSenderMachine[MaxNameSize + 1];
  TCHAR m_sTo[MaxNameSize + 1];
  TCHAR m_sMessage[MaxMessageSize + 1];
};

class CWinNotifyDoc : public CDocument
{
protected: // create from serialization only
	CWinNotifyDoc();
	DECLARE_DYNCREATE(CWinNotifyDoc)

public:
	virtual ~CWinNotifyDoc();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
  void CheckForNewMail();
  CString GetCurrentMessage() const;
  CString GetCurrentSender() const;
  CString GetCurrentSenderMachine() const;
  CString GetCurrentTo() const;
  CString GetCurrentMessageTime();
  INT_PTR GetNumberMessages() const { return m_Messages.GetSize(); };
  INT_PTR GetCurrentMessageCount() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnMessageClearAll();
	afx_msg void OnUpdateMessageClearAll(CCmdUI* pCmdUI);
	afx_msg void OnMessageDiscard();
	afx_msg void OnUpdateMessageDiscard(CCmdUI* pCmdUI);
	afx_msg void OnMessageNext();
	afx_msg void OnUpdateMessageNext(CCmdUI* pCmdUI);
	afx_msg void OnMessagePrevious();
	afx_msg void OnUpdateMessagePrevious(CCmdUI* pCmdUI);
	afx_msg void OnMessageSend();

	DECLARE_MESSAGE_MAP()

//Member variables
  CArray<CNotifyMessage, CNotifyMessage&> m_Messages;
  INT_PTR m_nCurrentMessageIndex;
  CServerMailslot m_ServerMailslot;
};

class CSendDlg : public CDialog
{
public:
	CSendDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_SEND };
	CString	m_sMessage;
	CString	m_sTo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
};
