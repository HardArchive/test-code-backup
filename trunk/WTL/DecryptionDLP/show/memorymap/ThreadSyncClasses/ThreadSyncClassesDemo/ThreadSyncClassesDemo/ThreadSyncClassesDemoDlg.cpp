
// ThreadSyncClassesDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadSyncClassesDemo.h"
#include "ThreadSyncClassesDemoDlg.h"
#include "afxdialogex.h"
#include "Synchronizer/MutexSynchronizer.h"
#include "Synchronizer/SemaphoreSynchronizer.h"
#include "Synchronizer/EventSynchronizer.h"
#include "Synchronizer/SingleObjectLock.h"
#include "Synchronizer/MultiObjectLock.h"
#include <memory>

#define MSG_PROGRESS_UPDATE WM_USER+1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CThreadSyncClassesDemoDlg dialog




CThreadSyncClassesDemoDlg::CThreadSyncClassesDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThreadSyncClassesDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreadSyncClassesDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS_DEMO, m_ProgressDemo);
}

BEGIN_MESSAGE_MAP(CThreadSyncClassesDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_MUTEX_TIMEOUT, &CThreadSyncClassesDemoDlg::OnBnClickedButtonMutexTimeout)
    ON_BN_CLICKED(IDC_BUTTON_MUTEX_ABANDONED, &CThreadSyncClassesDemoDlg::OnBnClickedButtonMutexAbandoned)
    ON_BN_CLICKED(IDC_BUTTON_MUTEX_ABANDONED, &CThreadSyncClassesDemoDlg::OnBnClickedButtonMutexAbandoned)
    ON_BN_CLICKED(IDC_BUTTON_MUTEX_WAITFAILED, &CThreadSyncClassesDemoDlg::OnBnClickedButtonMutexWaitfailed)
    ON_BN_CLICKED(IDC_BUTTON_SEM_TIMEOUT, &CThreadSyncClassesDemoDlg::OnBnClickedButtonSemTimeout)
    ON_BN_CLICKED(IDC_BUTTON_SEM_WAITFAILED, &CThreadSyncClassesDemoDlg::OnBnClickedButtonSemWaitfailed)
    ON_BN_CLICKED(IDC_BUTTON_MULTI_TIMEOUT, &CThreadSyncClassesDemoDlg::OnBnClickedButtonMultiTimeout)
    ON_BN_CLICKED(IDC_BUTTON_MULTI_ABANDONED, &CThreadSyncClassesDemoDlg::OnBnClickedButtonMultiAbandoned)
    ON_BN_CLICKED(IDC_BUTTON_MULTI_WAITFAILED, &CThreadSyncClassesDemoDlg::OnBnClickedButtonMultiWaitfailed)
    ON_BN_CLICKED(IDC_BUTTON_UI_MULTI_WAIT, &CThreadSyncClassesDemoDlg::OnBnClickedButtonUiMultiWait)
    ON_MESSAGE(MSG_PROGRESS_UPDATE,OnProgressUpdate)
END_MESSAGE_MAP()


// CThreadSyncClassesDemoDlg message handlers

BOOL CThreadSyncClassesDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    m_ProgressDemo.SetStep(5);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThreadSyncClassesDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CThreadSyncClassesDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThreadSyncClassesDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

enum ObjectType
{
    MUTEX,
    SEMAPHORE
};

// To pass the parameters to worker threads
struct THREAD_INFO
{
    ObjectType eObjType;
    CString csObjName;
}THREAD_INFO_t;

// Generates unique names for mutex kernel objects.
// This is required as thread is terminated using 
// TerminateThread to simulate the WAIT_ABANDONED error code,
// the next time we need a new name for test mutex.
CString CThreadSyncClassesDemoDlg::GetUniqueKernelObjName()
{
    UUID stUUID = {0};
    UuidCreate( &stUUID );
#ifdef UNICODE
#define RPC_TSTR RPC_WSTR
#else
#define RPC_TSTR RPC_CSTR
#endif
    RPC_TSTR pRPCTStr = 0;
    UuidToString(&stUUID,&pRPCTStr);
    CString csObjName(reinterpret_cast<LPTSTR>(pRPCTStr));
    RpcStringFree(&pRPCTStr);
    return csObjName;
}

// This thread acquires a mutex and will be terminated by the main
// thread using the TerminateThread API. This will cause the
// WAIT_ABANDONED error code to be returned by the WaitForSingleObject
// and will cause the SyncObjectWaitAbandoned to be thrown.
// Actually this is a unique feature of mutex object. This will not
// happen with other kernel objects. This is because the mutex remembers
// the ID of the owning thread and when this thread dies it will cause
// the system to return WAIT_ABANDONED.
UINT CThreadSyncClassesDemoDlg::WaitAbandonedThread(LPVOID lpVoid)
{
    try
    {
        THREAD_INFO* pstThreadInfo = reinterpret_cast<THREAD_INFO*>(lpVoid);
        std::auto_ptr<THREAD_INFO> ThreadInfoPtr(pstThreadInfo);
        
        // Acquire the mutex and then sleep infinitely till it is killed
        MutexSynchronizer MtxSync(0,FALSE,ThreadInfoPtr->csObjName);
        SingleObjectLock Lock(&MtxSync,1000);
        
        Sleep(INFINITE);
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
    return 0;
}

// This thread helps to generate the situations such as the WAIT_TIMEOUT
// When spawned it will acquire the kernel object and causes the main thread to
// timeout.
UINT CThreadSyncClassesDemoDlg::ExceptionSimulatorThread(LPVOID lpVoid)
{
    try
    {
        THREAD_INFO* pstThreadInfo = reinterpret_cast<THREAD_INFO*>(lpVoid);
        std::auto_ptr<THREAD_INFO> ThreadInfoPtr(pstThreadInfo);
        if(MUTEX == ThreadInfoPtr->eObjType)
        {
            MutexSynchronizer MtxSync(0,FALSE,ThreadInfoPtr->csObjName);
            SingleObjectLock Lock(&MtxSync,INFINITE);
            Sleep(INFINITE);
        }
        else
        if( SEMAPHORE == ThreadInfoPtr->eObjType )
        {
            SemaphoreSynchronizer SemSync(0,FALSE,1,1,1,0,ThreadInfoPtr->csObjName);
            SingleObjectLock Lock(&SemSync,INFINITE);
            Sleep(INFINITE);
        }
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
    return 0;
}

// It simulates the mutex wait timedout condition with the help
// of a child thread.
void CThreadSyncClassesDemoDlg::OnBnClickedButtonMutexTimeout()
{
    try
    {
        THREAD_INFO* pstThreadInfo = new THREAD_INFO;
        pstThreadInfo->eObjType = MUTEX;
        pstThreadInfo->csObjName = GetUniqueKernelObjName();

        // Let thread acquire the mutex
        CWinThread* pTimeoutThread = AfxBeginThread(ExceptionSimulatorThread,pstThreadInfo,0,0);
        AfxMessageBox(_T("Thread acquired lock...click OK to timeout"));
        
        // Try to acquire the mutex and get timeout
        MutexSynchronizer MtxSync(0,FALSE,pstThreadInfo->csObjName);
        SingleObjectLock Lock(&MtxSync,100);
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
}

// Simulate the mutex wait abandoned condition
void CThreadSyncClassesDemoDlg::OnBnClickedButtonMutexAbandoned()
{
    CString csMutexName = GetUniqueKernelObjName();
    
    THREAD_INFO* pstThreadInfo1 = new THREAD_INFO;
    pstThreadInfo1->eObjType = MUTEX;
    pstThreadInfo1->csObjName = csMutexName;
    
    // Let the worker thread to acquire the mutex...
    CWinThread* pWaitAbandonedThread = AfxBeginThread(WaitAbandonedThread,pstThreadInfo1,0,0);
    AfxMessageBox(_T("Please give time for another thread to acquire lock...now poceed"));
    
    THREAD_INFO* pstThreadInfo2 = new THREAD_INFO;
    pstThreadInfo2->eObjType = MUTEX;
    pstThreadInfo2->csObjName = csMutexName;

    // Let the other thread to try to acquire the mutex...
    CWinThread* pExceptionSimulatorThread = AfxBeginThread(ExceptionSimulatorThread,pstThreadInfo2,
                                                           0,0,0);
    
    // Now, terminate the first thread that owns the mutex...
    // this will simulate WAIT_ABANDONED
    ::TerminateThread(pWaitAbandonedThread->m_hThread,0);
}

// This will simulate the invalid mutex handle case
void CThreadSyncClassesDemoDlg::OnBnClickedButtonMutexWaitfailed()
{
    try
    {
        MutexSynchronizer MtxSync(0,FALSE,GetUniqueKernelObjName());
        // Attach a NULL handle and try to acquire the mutex.
        // This will generate the WaitFailed exception
        MtxSync.Attach(0);
        SingleObjectLock Lock(&MtxSync,100);
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
}

// This will simulate the semaphore timeout condition
void CThreadSyncClassesDemoDlg::OnBnClickedButtonSemTimeout()
{
    try
    {
        CString csSemName = _T("TEST_SEMAPHORE");
        THREAD_INFO* pstThreadInfo = new THREAD_INFO;
        pstThreadInfo->eObjType = SEMAPHORE;
        pstThreadInfo->csObjName = csSemName;

        //Let the worker thread acquire the semaphore...
        CWinThread* pTimeoutThread = AfxBeginThread(ExceptionSimulatorThread,pstThreadInfo,0,0);
        AfxMessageBox(_T("Thread acquired lock...click OK to timeout"));
        
        // Now, try to acquire the semaphore with a timeout.
        // This will generate WaitTimeout exception
        SemaphoreSynchronizer SemSync(0,FALSE,1,1,1,0,csSemName);
        SingleObjectLock Lock(&SemSync,100);
     
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
}

// Attach a NULL semaphore handle to simulate the wait failed
void CThreadSyncClassesDemoDlg::OnBnClickedButtonSemWaitfailed()
{
    try
    {
        SemaphoreSynchronizer SemSync(0,FALSE,1,1,1,0,_T("TEST_SEMAPHORE"));
        SemSync.Attach(0);
        SingleObjectLock Lock(&SemSync,100);
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
}

// This will simulate the WaitForMultipleObjects timeout condition
void CThreadSyncClassesDemoDlg::OnBnClickedButtonMultiTimeout()
{
    try
    {
        CString csMutexName = GetUniqueKernelObjName();
        
        THREAD_INFO* pstThreadInfo = new THREAD_INFO;
        pstThreadInfo->eObjType = MUTEX;
        pstThreadInfo->csObjName = csMutexName;

        // Let the worker thread acquire the mutex...
        CWinThread* pTimeoutThread = AfxBeginThread(ExceptionSimulatorThread,pstThreadInfo,0,0);
        AfxMessageBox(_T("Let the other thread acquire the mutex and we time out...now close"));

        // Prepare mutex and semaphore objects
        typedef std::auto_ptr<SynchronizerBase> SyncObjPtr;
        SyncObjPtr pMutexPtr(new MutexSynchronizer(0,FALSE,csMutexName));
        SyncObjPtr pSemPtr(new SemaphoreSynchronizer(0,FALSE,1,1,1,0,_T("TEST_SEMAPHORE")));
        
        // Add mutex and semaphore
        LockHolder lhLockHolderObj;
        lhLockHolderObj.AddSynchronizer(pMutexPtr.get());
        lhLockHolderObj.AddSynchronizer(pSemPtr.get());
        
        // Now, try for a WaitForMultipleObjects.
        // It will generate WaitTimeout exception.
        MultiObjectLock Lock(&lhLockHolderObj,TRUE,100,FALSE,0); 
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
}

// This thread will help to generate the WAIT_ABADONED_0+ error codes.
// It will lead to WaitAbandoned exception
UINT CThreadSyncClassesDemoDlg::MultiWaitAbandonedThread(LPVOID lpVoid)
{
    try
    {
        THREAD_INFO* pstThreadInfo = reinterpret_cast<THREAD_INFO*>(lpVoid);
        std::auto_ptr<THREAD_INFO> ThreadInfoPtr(pstThreadInfo);
        
        // Prepare a mutex
        typedef std::auto_ptr<SynchronizerBase> SyncObjPtr;
        SyncObjPtr pMutexPtr(new MutexSynchronizer(0,FALSE,pstThreadInfo->csObjName));
        
        // Add the mutex and acquire the lock and sleep indefinitely.
        // This thread will be killed, that will cause WAIT_ABANDONED_0+
        // to be returned. The other thread will get the WaitAbandoned exception
        LockHolder lhLockHolderObj;
        lhLockHolderObj.AddSynchronizer(pMutexPtr.get());
        MultiObjectLock Lock(&lhLockHolderObj,TRUE,100,FALSE,0); 
        Sleep(INFINITE);
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }
    return 0;
}

// The button to simulate the WaitForMultipleObjects to return WAIT_ABANDONED_0+
void CThreadSyncClassesDemoDlg::OnBnClickedButtonMultiAbandoned()
{
    CString csMutexName = GetUniqueKernelObjName();
    
    // Let one of the worker threads acquire mutex
    THREAD_INFO* pstThreadInfo1 = new THREAD_INFO;
    pstThreadInfo1->csObjName = csMutexName;
    CWinThread* pWaitAbandonedThread1 = AfxBeginThread(MultiWaitAbandonedThread,pstThreadInfo1,0,0);
    AfxMessageBox(_T("Please give time for another thread to acquire lock...now poceed"));
    
    // Now, let another thread to try the same mutex...
    // It will be waiting to acquire the mutex.
    THREAD_INFO* pstThreadInfo2 = new THREAD_INFO;
    pstThreadInfo2->csObjName = csMutexName;
    CWinThread* pWaitAbandonedThread2 = AfxBeginThread(MultiWaitAbandonedThread,pstThreadInfo2,0,0);

    // Now, terminate the thread which acquired the mutex.
    // It generate the WaitAbandoned exception in second thread.
    ::TerminateThread(pWaitAbandonedThread1->m_hThread,0);
}

// Attach a NULL handle to simulate the WAIT_FAILED.
void CThreadSyncClassesDemoDlg::OnBnClickedButtonMultiWaitfailed()
{
    try
    {
        typedef std::auto_ptr<SynchronizerBase> SyncObjPtr;
        SyncObjPtr pMutexPtr(new MutexSynchronizer(0,FALSE,GetUniqueKernelObjName()));
        
        LockHolder lhLockHolderObj;
        lhLockHolderObj.AddSynchronizer(pMutexPtr.get());
        pMutexPtr->Attach(0);
        MultiObjectLock Lock(&lhLockHolderObj,TRUE,100,FALSE,0); 
    }
    catch (SyncObjectWaitTimeOut& WaitTimeOut)
    {
        AfxMessageBox(WaitTimeOut.GetDescription());
    }
    catch(SyncObjectWaitFailed& WaitFailed)
    {
        AfxMessageBox(WaitFailed.GetDescription());
    }
    catch(SyncObjectWaitAbandoned& WaitAbandoned)
    {
        AfxMessageBox(WaitAbandoned.GetDescription());
    }
    catch(GeneralException& GenException)
    {
        AfxMessageBox(GenException.GetDescription());
    }

}

const int PROGRESS_MAX = 10;

// The worker thread that will send progress messages to the main thread
// while it is waiting on MsgWaitForMultipleObjects.
// Finally, it will wake the event that the main thread is waiting on.
UINT CThreadSyncClassesDemoDlg::UIWaitDemoThread(LPVOID lpVoid)
{
    for( int nIdx = 0; nIdx < PROGRESS_MAX; ++nIdx )
    {
        Sleep(300);
        AfxGetApp()->GetMainWnd()->SendMessage(MSG_PROGRESS_UPDATE,0,0);
    }
    EventSynchronizer* pEvent = reinterpret_cast<EventSynchronizer*>(lpVoid);
    if( pEvent )
    {
        pEvent->SetEvent();
    }
    return 0;
}

// The handler corresponding to MSG_PROGRESS_UPDATE
LRESULT CThreadSyncClassesDemoDlg::OnProgressUpdate(WPARAM,LPARAM)
{
    m_ProgressDemo.StepIt();
    return 0;
}

// It will simulate the MsgWaitForMultipleObjects
void CThreadSyncClassesDemoDlg::OnBnClickedButtonUiMultiWait()
{
    m_ProgressDemo.SetRange(0,PROGRESS_MAX);
    
    EventSynchronizer EventSync(0,FALSE,TRUE,FALSE,0);
    typedef std::auto_ptr<SynchronizerBase> SyncObjPtr;
    SyncObjPtr pEventPtr(new EventSynchronizer(0,FALSE,TRUE,FALSE,0));
    
    CWinThread* pUIWaitThread = AfxBeginThread(UIWaitDemoThread,pEventPtr.get(),0,0);
    
    LockHolder lhLockHolderObj;
    lhLockHolderObj.AddSynchronizer(pEventPtr.get());
    for(;;)
    {
        // Call MsgWaitForMultipleObjects and allow to pass input events.
        MultiObjectLock Lock(&lhLockHolderObj,FALSE,INFINITE,TRUE,QS_ALLINPUT); 
        // If the reason to return is not a message, it is the case when the event is
        // signalled, then break the loop.
        // Ensure the messages are dispatched.
        AfxGetApp()->PumpMessage();
        if( !Lock.HasMessageInQueue() )
        {
            AfxMessageBox(_T("Wait complete"));
            break;
        }
    }
}
