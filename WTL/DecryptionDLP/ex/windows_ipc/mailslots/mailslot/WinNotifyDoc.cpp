#include "stdafx.h"
#include "WinNotify.h"
#include "WinNotifyDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CWinNotifyDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinNotifyDoc, CDocument)
	ON_COMMAND(ID_MESSAGE_CLEARALL, OnMessageClearAll)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_CLEARALL, OnUpdateMessageClearAll)
	ON_COMMAND(ID_MESSAGE_DISCARD, OnMessageDiscard)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_DISCARD, OnUpdateMessageDiscard)
	ON_COMMAND(ID_MESSAGE_NEXT, OnMessageNext)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_NEXT, OnUpdateMessageNext)
	ON_COMMAND(ID_MESSAGE_PREVIOUS, OnMessagePrevious)
	ON_UPDATE_COMMAND_UI(ID_MESSAGE_PREVIOUS, OnUpdateMessagePrevious)
	ON_COMMAND(ID_MESSAGE_SEND, OnMessageSend)
END_MESSAGE_MAP()

CWinNotifyDoc::CWinNotifyDoc() : m_nCurrentMessageIndex(0)
{
}

CWinNotifyDoc::~CWinNotifyDoc()
{
}

BOOL CWinNotifyDoc::OnNewDocument()
{
  //Let the base class do its thing
	if (!CDocument::OnNewDocument())
		return FALSE;

  //Create the server mailslot to listen for incoming messages
  if (!m_ServerMailslot.Open(_T("WinNot\\Server")))
  {
    AfxMessageBox(IDP_FAIL_CREATE_SERVER_MAILSLOT, MB_OK | MB_ICONSTOP);
    return FALSE;
  }

	return TRUE;
}

INT_PTR CWinNotifyDoc::GetCurrentMessageCount() const
{
  //Return the current number of messages
	if (m_Messages.GetSize())
    return m_nCurrentMessageIndex+1;
  else
    return 0;
}

void CWinNotifyDoc::Serialize(CArchive& /*ar*/)
{
  //We do not do anything with the hard disk
}

#ifdef _DEBUG
void CWinNotifyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinNotifyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CWinNotifyDoc::OnMessageClearAll() 
{
  //remove all messages from the array and inform all views
  m_Messages.RemoveAt(0, m_Messages.GetSize());
  m_nCurrentMessageIndex = 0;
  UpdateAllViews(NULL);
}

void CWinNotifyDoc::OnUpdateMessageClearAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Messages.GetSize() > 0);
}

void CWinNotifyDoc::OnMessageDiscard() 
{
  //Remove the current message from the array and inform all views
	m_Messages.RemoveAt(m_nCurrentMessageIndex);
  if (m_nCurrentMessageIndex > m_Messages.GetUpperBound())
    m_nCurrentMessageIndex = m_Messages.GetUpperBound();
	UpdateAllViews(NULL);
}

void CWinNotifyDoc::OnUpdateMessageDiscard(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_Messages.GetSize() > 0);		
}

void CWinNotifyDoc::OnMessageNext() 
{
  //Move to the next message and inform all views
	++m_nCurrentMessageIndex;
  UpdateAllViews(NULL);
}

void CWinNotifyDoc::OnUpdateMessageNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nCurrentMessageIndex < m_Messages.GetUpperBound());
}

void CWinNotifyDoc::OnMessagePrevious() 
{
  //Move to the previous message and inform all views
	--m_nCurrentMessageIndex;
  UpdateAllViews(NULL);
}

void CWinNotifyDoc::OnUpdateMessagePrevious(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_nCurrentMessageIndex > 0);
}

void CWinNotifyDoc::OnMessageSend() 
{
  //Send a message by opening a client side mailslot and writing to it
	CSendDlg dlg;
  if (dlg.DoModal() == IDOK)
  {
    //Display a wait cursor
    CWaitCursor wait;
  
    CClientMailslot mailslot;
    if (!mailslot.Open(dlg.m_sTo, _T("WinNot\\Server")))
    {
      AfxMessageBox(IDP_FAIL_CREATE_CLIENT_MAILSLOT, MB_OK | MB_ICONEXCLAMATION);
      return;
    }

    //create the message
    CNotifyMessage msg;
    _tcscpy_s(msg.m_sMessage, sizeof(msg.m_sMessage)/sizeof(TCHAR), dlg.m_sMessage);

    TCHAR sUserName[CNotifyMessage::MaxNameSize];
    DWORD dwUserNameSize = CNotifyMessage::MaxNameSize;
    if (GetUserName(sUserName, &dwUserNameSize))
      _tcscpy_s(msg.m_sSenderName, sizeof(msg.m_sSenderName)/sizeof(TCHAR), sUserName);
    else
      _tcscpy_s(msg.m_sSenderName, sizeof(msg.m_sSenderName)/sizeof(TCHAR), _T("Unknown"));

    TCHAR sComputerName[CNotifyMessage::MaxNameSize];
    DWORD dwComputerNameSize = CNotifyMessage::MaxNameSize;
    if (GetComputerName(sComputerName, &dwComputerNameSize))
      _tcscpy_s(msg.m_sSenderMachine, sizeof(msg.m_sSenderMachine)/sizeof(TCHAR), sComputerName);
    else
      _tcscpy_s(msg.m_sSenderMachine, sizeof(msg.m_sSenderMachine)/sizeof(TCHAR), _T("Unknown"));

    _tcscpy_s(msg.m_sTo, sizeof(msg.m_sTo)/sizeof(TCHAR), dlg.m_sTo);
    GetLocalTime(&msg.m_SentTime);

    //Send the message
    DWORD dwBytesWritten = 0;
    DWORD dwBytesToWrite = sizeof(CNotifyMessage);
    if (!mailslot.Write(&msg, dwBytesToWrite, dwBytesWritten) || (dwBytesToWrite != dwBytesWritten))
    {
      //Display the error
      DWORD dwError = GetLastError();
      
	    LPTSTR lpBuffer;
	    DWORD dwReturn = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			                               NULL,  dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			                               reinterpret_cast<LPTSTR>(&lpBuffer), 0, NULL);

      CString sError;
	    if (dwReturn != 0)
	    {
        sError.Format(_T("%d, %s"), dwError, lpBuffer);
		    LocalFree(lpBuffer);
	    }
	    else
        sError.Format(_T("%d"), dwError);
    
      CString sMsg;
      AfxFormatString1(sMsg, IDP_FAIL_SEND_MESSAGE, sError);
      AfxMessageBox(sMsg, MB_OK | MB_ICONEXCLAMATION);
    }
  }
}

void CWinNotifyDoc::CheckForNewMail()
{
  if (m_ServerMailslot.IsOpen())
  {
    //Read all the messages waiting in 
    //the server mailslot
    while (m_ServerMailslot.MessageWaiting())
    {
      //Work out the size of the message waiting
      DWORD dwMsgSize = m_ServerMailslot.SizeOfWaitingMessage();
      
      //Allocate some memory to hold the message
      ATL::CHeapPtr<BYTE> msgData;
      if (msgData.Allocate(dwMsgSize))
      {
        //Read the message from the mailslot
        DWORD dwNumberOfBytesRead = 0;
        if (m_ServerMailslot.Read(msgData, dwMsgSize, dwNumberOfBytesRead))
        {
          //Add it to the array if a valid size
          if (dwNumberOfBytesRead == sizeof(CNotifyMessage))
          {
            CNotifyMessage msg;
            memcpy(&msg, msgData, dwNumberOfBytesRead);
            m_Messages.Add(msg);
            m_nCurrentMessageIndex = m_Messages.GetUpperBound();
            UpdateAllViews(NULL);
            MessageBeep(MB_ICONEXCLAMATION);
          }
        }
      }
    }
  }
}

CString CWinNotifyDoc::GetCurrentMessage() const 
{
  //What will be return value from this function
  CString sText;

  if (m_Messages.GetSize())
    sText = m_Messages.GetAt(m_nCurrentMessageIndex).m_sMessage;
  
  return sText;
}

CString CWinNotifyDoc::GetCurrentSender() const
{
  //What will be the return value from this function
  CString sText;

  if (m_Messages.GetSize())
    sText = m_Messages.GetAt(m_nCurrentMessageIndex).m_sSenderName;

  return sText;
}

CString CWinNotifyDoc::GetCurrentSenderMachine() const
{
  //What will be the return value from this function
  CString sText;

  if (m_Messages.GetSize())
    sText = m_Messages.GetAt(m_nCurrentMessageIndex).m_sSenderMachine;

  return sText;
}

CString CWinNotifyDoc::GetCurrentTo() const
{
  //What will be the return value from this function
  CString sText;

  if (m_Messages.GetSize())
    sText = m_Messages.GetAt(m_nCurrentMessageIndex).m_sTo;
  
  return sText;
}


CString CWinNotifyDoc::GetCurrentMessageTime()
{
  //What will be the return value from this function
  CString sText;

  if (m_Messages.GetSize())
  {
    TCHAR szDateStr[64];
    TCHAR szTimeStr[64];
		CNotifyMessage& curMessage = m_Messages.ElementAt(m_nCurrentMessageIndex);

    GetDateFormat(LOCALE_USER_DEFAULT, 0, &curMessage.m_SentTime, NULL, szDateStr, sizeof(szDateStr)/sizeof(TCHAR));
    GetTimeFormat(LOCALE_USER_DEFAULT, 0, &curMessage.m_SentTime, NULL, szTimeStr, sizeof(szTimeStr)/sizeof(TCHAR));
    sText = CString(szDateStr) + CString(_T(" ")) + szTimeStr;
  }
  
  return sText;
}


CSendDlg::CSendDlg(CWnd* pParent)	: CDialog(CSendDlg::IDD, pParent)
{
}

void CSendDlg::DoDataExchange(CDataExchange* pDX)
{
  //Let the base class do its thing
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_TO, m_sTo);
	DDX_Text(pDX, IDC_MESSAGE, m_sMessage);
  pDX->PrepareEditCtrl(IDC_TO);
  DDV_MaxChars(pDX, m_sTo, CNotifyMessage::MaxNameSize);
  pDX->PrepareEditCtrl(IDC_MESSAGE);
  DDV_MaxChars(pDX, m_sMessage, CNotifyMessage::MaxMessageSize);
}


BEGIN_MESSAGE_MAP(CSendDlg, CDialog)
END_MESSAGE_MAP()
