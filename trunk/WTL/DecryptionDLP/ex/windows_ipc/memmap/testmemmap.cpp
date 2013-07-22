#include "stdafx.h"
#include "testmemmap.h"

const int MAX_EDIT_TEXT = 100;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTestmemmapApp, CWinApp)
END_MESSAGE_MAP()

CTestmemmapApp::CTestmemmapApp()
{
}

CTestmemmapApp theApp;

BOOL CTestmemmapApp::InitInstance()
{
  //Form a path to the test txt file we will be operating on
  TCHAR pszTestFile[_MAX_PATH];
  TCHAR pszUpperCaseTestFile[_MAX_PATH];
  TCHAR pszLowerCaseTestFile[_MAX_PATH];
  GetModuleFileName(NULL, pszTestFile, _MAX_PATH);
  TCHAR pszDrive[_MAX_DRIVE];
  TCHAR pszDir[_MAX_DIR];
  _tsplitpath_s(pszTestFile, pszDrive, sizeof(pszDrive)/sizeof(TCHAR), pszDir, sizeof(pszDir)/sizeof(TCHAR), NULL, 0, NULL, 0);
  _tmakepath_s(pszTestFile, sizeof(pszTestFile)/sizeof(TCHAR), pszDrive, pszDir, _T("input"), _T("txt"));
  _tmakepath_s(pszUpperCaseTestFile, sizeof(pszUpperCaseTestFile)/sizeof(TCHAR), pszDrive, pszDir, _T("outputUpperCase"), _T("txt"));
  _tmakepath_s(pszLowerCaseTestFile, sizeof(pszLowerCaseTestFile)/sizeof(TCHAR), pszDrive, pszDir, _T("outputLowerCase"), _T("txt"));

  //First test out the class when we want to map a file
  //You might want to step through this code to test it out
  CMemMapFile mmf;
  BOOL bSuccess = mmf.MapFile(pszTestFile, FALSE);
  VERIFY(bSuccess);
  LPVOID pData = mmf.Open();
  bSuccess = mmf.Flush();
  bSuccess = mmf.Close();
  mmf.UnMap();

  //Now a real demonstration of Memory mapped file usage. Make the sample input.txt file 
  //uppercase. Error checking is ignored / implemented by using VERIFY for demonstration purposes
  //The real power of memory mapped files is that this demonstration would work just as efficiently 
  //with a 100 MB file as it does with just a small text file. Note that because these samples uses 
  //CharUpperBuffA and ChatLowerBuffA, the file size must be less than 4GB since we cast to a DWORD 
  //sizes parameter 
  VERIFY(CopyFile(pszTestFile, pszUpperCaseTestFile, FALSE));
  CMemMapFile mmf3;
  VERIFY(mmf3.MapFile(pszUpperCaseTestFile));
  pData = mmf3.Open();
  VERIFY(pData);
  CFileStatus fs;
  VERIFY(CFile::GetStatus(pszTestFile, fs));
  ASSERT(fs.m_size <= ULONG_MAX);
  CharUpperBuffA(static_cast<LPSTR>(pData), static_cast<DWORD>(fs.m_size));
  mmf3.UnMap();
  CString sMsg;
  sMsg.Format(_T("The file %s should be the uppercase version of %s"), pszUpperCaseTestFile, pszTestFile);
  AfxMessageBox(sMsg);

  //Also try lowercase version
  VERIFY(CopyFile(pszTestFile, pszLowerCaseTestFile, FALSE));
  CMemMapFile mmf4;
  VERIFY(mmf4.MapFile(pszLowerCaseTestFile));
  pData = mmf4.Open();
  VERIFY(pData);
  VERIFY(CFile::GetStatus(pszTestFile, fs));
  VERIFY(fs.m_size <= ULONG_MAX);
  CharLowerBuffA(static_cast<LPSTR>(pData), static_cast<DWORD>(fs.m_size));
  mmf4.UnMap();
  sMsg.Format(_T("The file %s should be the lowercase version of %s"), pszLowerCaseTestFile, pszTestFile);
  AfxMessageBox(sMsg);

  //Finally a demonstration of the shared memory capabilities of 
  //memory mapped files. For this demo, we will bring up a UI which
  //will take whatever is put into its edit box and transfer it into 
  //shared memory. Then if you bring up another version of testmemmap, 
  //you will see this value being reflecting into it.
  //periodically
  CDialog1 dlg;
  dlg.DoModal();

	return FALSE;
}


CDialog1::CDialog1(CWnd* pParent)	: CDialog(CDialog1::IDD, pParent)
{
}

void CDialog1::DoDataExchange(CDataExchange* pDX)
{
  //Let the base class do its thing
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
}

BEGIN_MESSAGE_MAP(CDialog1, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CDialog1::OnInitDialog() 
{
  //Let the base class do its thing
	CDialog::OnInitDialog();

  //Limit the text to the size that can be held in the MMF
  m_ctrlEdit.LimitText(MAX_EDIT_TEXT);

  //Check to see if we are the first instance
  //of this program running. In the process testing
  //the MapExistingMemory method
	if (m_mmf.MapExistingMemory(_T("MMFTEST"), _T("MMFTEST_MUTEX"), (MAX_EDIT_TEXT+1)*sizeof(TCHAR)))
    AfxMessageBox(_T("This program is already running"));
  else
  {
    AfxMessageBox(_T("This program is the first instance"));

    //map the shared memory, 
	  if (m_mmf.MapMemory(_T("MMFTEST"), _T("MMFTEST_MUTEX"), (MAX_EDIT_TEXT+1)*sizeof(TCHAR)))
    {
      //Set the initial text which will go into the MMF
      m_ctrlEdit.SetWindowText(_T("Initial Text"));
    }
    else
    {
      AfxMessageBox(_T("Failed to map shared memory"));
      PostMessage(WM_CLOSE);
    }
  }
  
  //Create a 1 second timer to allow us to update the edit box
	m_nTimer = SetTimer(1, 1000, NULL);
	
	return TRUE;
}

void CDialog1::OnChangeEdit1() 
{
	TCHAR sText[MAX_EDIT_TEXT+1];
  m_ctrlEdit.GetWindowText(sText, MAX_EDIT_TEXT);
  LPVOID lpData = m_mmf.Open();
  if (lpData)
    _tcscpy_s(static_cast<TCHAR*>(lpData), sizeof(sText)/sizeof(TCHAR), sText);
  m_mmf.Close();
}

void CDialog1::OnDestroy() 
{
  KillTimer(m_nTimer);

  //Let the base class do its thing
	CDialog::OnDestroy();
}

void CDialog1::OnTimer(UINT_PTR /*nIDEvent*/) 
{
  //Retreive the contents of the MMF
	TCHAR sNewText[MAX_EDIT_TEXT+1];
  _tcscpy_s(sNewText, sizeof(sNewText)/sizeof(TCHAR), _T(""));
	LPVOID lpData = m_mmf.Open();
  if (lpData)
    _tcscpy_s(sNewText, sizeof(sNewText)/sizeof(TCHAR), static_cast<TCHAR*>(lpData));
  m_mmf.Close();

  //update the edit control if the text has changed
  CString sCurrentText;
  m_ctrlEdit.GetWindowText(sCurrentText);
  if (sCurrentText != sNewText)
    m_ctrlEdit.SetWindowText(sNewText);
}
