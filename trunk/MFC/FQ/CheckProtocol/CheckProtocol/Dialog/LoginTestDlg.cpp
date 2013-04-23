// LoginTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "LoginTestDlg.h"
#include "CaptruePackInfoDlg.h"

// CLoginTestDlg 对话框
CLoginTestDlg  *CLoginTestDlg::m_pThis = NULL;

IMPLEMENT_DYNAMIC(CLoginTestDlg, CDialog)

CLoginTestDlg::CLoginTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginTestDlg::IDD, pParent)
{
	m_bSelectState = TRUE;
	m_bShowMode = TRUE;
	m_PackServiceType = CAPTURE_PACK_INFO;
}

CLoginTestDlg::~CLoginTestDlg()
{
	m_pThis = NULL;
}

void CLoginTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOGINSTAT, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO_SETADAPTER, m_ComboBox);
}


BEGIN_MESSAGE_MAP(CLoginTestDlg, CDialog)
	ON_BN_CLICKED(ID_BUTTON_PACKINFO, &CLoginTestDlg::OnBnClickedButtonPackinfo)
	ON_BN_CLICKED(ID_BUTTON_TEST, &CLoginTestDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDCANCEL, &CLoginTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BUTTONG_SET, &CLoginTestDlg::OnBnClickedButtongSet)
	ON_BN_CLICKED(ID_BUTTON_STOP, &CLoginTestDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CLoginTestDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_CAPTUREMODE, &CLoginTestDlg::OnBnClickedButtonShowCapturemode)
END_MESSAGE_MAP()


// CLoginTestDlg 消息处理程
BOOL CLoginTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pThis = this;
	GetDlgItem(ID_BUTTON_TEST)->EnableWindow(FALSE);
	GetDlgItem(ID_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(ID_BUTTON_PACKINFO)->EnableWindow(FALSE);

	((CButton *)GetDlgItem(IDC_RADIO_ONLINE_CAPTURE))->SetCheck(1);

	vector <string>::iterator pVectAdaterName = CGlobalData::m_vectAdapterName.begin();
	for(;pVectAdaterName != CGlobalData::m_vectAdapterName.end(); ++pVectAdaterName)
	{
		CString strAdapterName(pVectAdaterName->c_str());
		m_ComboBox.AddString(strAdapterName);
	}

	if(!AddListItem())		return FALSE;
	int nRow = 0;
	map<string,PACKRULEFILE>::iterator pPackRule = CGlobalData::m_mapPackRuleFileInfo.begin();
	for(pPackRule;pPackRule != CGlobalData::m_mapPackRuleFileInfo.end();++pPackRule)
	{
		AddListInfo((pPackRule->second.Item),nRow);
		nRow++;
	}
	return TRUE;
}

BOOL CLoginTestDlg::AddListItem()
{
//	m_ListCtrl.ShowScrollBar(SB_HORZ,FALSE);
// 	m_ListCtrl.ShowScrollBar(SB_VERT,TRUE);
// 	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES|LVS_EX_TRACKSELECT|\
// 								LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVN_ODFINDITEM);
	m_ListCtrl.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP \
		                         | LVS_EX_ONECLICKACTIVATE | LVS_EX_CHECKBOXES | LVN_ODFINDITEM);
	if(-1 == m_ListCtrl.InsertColumn(0,_T(""),LVCFMT_LEFT,20))                               return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(1,_T("ItemName"),LVCFMT_LEFT,130))			             return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(2,_T("WebState"), LVCFMT_CENTER,105))		             return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(3,_T("PackState"),LVCFMT_CENTER,105))	       	         return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(4,_T("Remark"),LVCFMT_CENTER,105))	                     return FALSE;
	return TRUE;
}

void CLoginTestDlg::AddListInfo(char *pItemName,int nRow)
{
	m_ListCtrl.InsertItem(nRow,_T(""));
	CString strItemName(pItemName);
	m_ListCtrl.SetItemText(nRow,1,strItemName);
}

void CLoginTestDlg::OnBnClickedButtonPackinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CCaptruePackInfoDlg CaptrueDlg;
	CaptrueDlg.DoModal();
}

void CLoginTestDlg::OnBnClickedButtongSet()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton *)GetDlgItem(IDC_CHECK_CLEAR_INTERNET))->GetCheck())                 //Xt:清理IE
	{
		TCHAR szPath[MAX_PATH];
		DeleteUrlCache(emFile);
		if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_INTERNET_CACHE, FALSE))
		{  
			EmptyDirectory(szPath);					                                 //得到临时目录，并清空它.
		}
	}

	if(((CButton *)GetDlgItem(IDC_CHECK_CLEAR_COOKIE))->GetCheck())                  //Xt:清理COOKIE
	{	
		TCHAR szPath[MAX_PATH];
		DeleteUrlCache(emCookie);
		if (SHGetSpecialFolderPath(NULL, szPath, CSIDL_COOKIES, FALSE))
		{  
			EmptyDirectory(szPath);                                                  //得到临时目录，并清空它.
		}
	}

	int nNum = m_ComboBox.GetCurSel();                    
	CString strAdapterName;
	if(CB_ERR == nNum)
	{
		MessageBox(_T("请选择网卡!"));
		return;
	}
	else
	{	
		m_ComboBox.GetLBText(nNum,strAdapterName);                                   //Xt:设置网卡信息
		CChangeCode cCode;
		char adapterName[1024]  = {0};
		cCode.UnicodeToGB2312Code(adapterName,strAdapterName.GetBuffer());
	  
		if(!m_packThread.SetAdapterInfo(adapterName))
		{
			MessageBox(_T("设置网卡错误!"));
			return;
		}
		for(int i = 0;i<m_ListCtrl.GetItemCount();i++)
		{	
			m_pThis->m_ListCtrl.SetItemText(i,2,_T(""));
            m_pThis->m_ListCtrl.SetItemText(i,3,_T(""));
			m_pThis->m_ListCtrl.SetItemText(i,4,_T(""));
		}
		
		CGlobalData::m_vtOfflinePackPath.clear();                                     //Xt:先清空上次保存的离线包路径
		m_PackServiceType = CAPTURE_PACK_INFO;

		if(((CButton *)GetDlgItem(IDC_RADIO_OFFLINE_CAPTURE))->GetCheck())            //Xt:获取离线包路径
		{
			char strPath[MAX_PATH] = {0};
			CString strError;
			CFileDialog  fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
				                 _T("pcap FILE(*.PCAP)|*.PCAP||"),NULL);
			if(fileDlg.DoModal() == IDOK)
			{
				CString str = fileDlg.GetPathName();
				char strPath[MAX_PATH] = {0};
				CChangeCode::UnicodeToGB2312Code(strPath,fileDlg.GetPathName().GetBuffer());
				if(strlen(strPath)<3)
				{
					MessageBox(_T("获取离线包错误!"));
					return;
				}
				CGlobalData::m_vtOfflinePackPath.push_back(strPath);
			}
			m_PackServiceType |= CAPTURE_OFFLINE_PACK;
		}
		else if(((CButton *)GetDlgItem(IDC_RADIO_OFFLINE_CAPTUREFILE))->GetCheck())    //Xt: 选择离线文件夹   
		{
			HANDLE hFind;
			WIN32_FIND_DATAA winFindData;

			char strFindPath[MAX_PATH] = {0};
			sprintf_s(strFindPath,sizeof(strFindPath),"%s\\*.pcap",CGlobalData::m_strOfflinePackPath);       //Xt: 设置搜索路径

			hFind = FindFirstFileA(strFindPath,&winFindData);
			if(INVALID_HANDLE_VALUE != hFind)
			{
				do 
				{
					sprintf_s(strFindPath,sizeof(strFindPath),"%s\\%s",CGlobalData::m_strOfflinePackPath,winFindData.cFileName);
					CGlobalData::m_vtOfflinePackPath.push_back(strFindPath);
				} 
				while(::FindNextFileA(hFind,&winFindData));
			}
			else
			{
				CString strError(CGlobalData::PrintLogInfo("CLoginTestDlg","获取离线文件夹信息错误!",'E',GetLastError()));
				MessageBox(strError);
				return;
			}
			FindClose(hFind);
			m_PackServiceType |= CAPTURE_OFFLINE_PACK;
		}
		else
		{
			m_PackServiceType |= CAPTURE_INLINE_PACK;
		}
		GetDlgItem(ID_BUTTONG_SET)->EnableWindow(FALSE);
		GetDlgItem(ID_BUTTON_TEST)->EnableWindow(TRUE);
		GetDlgItem(ID_BUTTON_PACKINFO)->EnableWindow(FALSE);
	}
}

void CLoginTestDlg::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	remove(CGlobalData::m_strInterceptedInfoPath);                             //Xt: 删除上次解析的文件信息
	BOOL bWeb=	((CButton *)GetDlgItem(IDC_CHECK_NOWEB))->GetCheck();
	BOOL bPack = ((CButton *)GetDlgItem(IDC_CHECK_NOPACK))->GetCheck();
	BOOL bMatch = ((CButton *)GetDlgItem(IDC_CHECK_NOMATCH))->GetCheck();
	if( bWeb && bPack)
	{
		MessageBox(_T("无可执行操作!"));
		return;
	}
	if(CAPTURE_INLINE_PACK == (m_PackServiceType & CAPTURE_INLINE_PACK))       //Xt: 判断在(or离线捕获处理)
	{
		CChangeCode cCode;
		CString strItemName;
		char pFindName[20] = {0};
		for(int i = 0;i<m_ListCtrl.GetItemCount();i++)
		{
			if(m_ListCtrl.GetCheck(i))
			{
				strItemName = m_ListCtrl.GetItemText(i,1);
				cCode.UnicodeToGB2312Code(pFindName,strItemName.GetBuffer());
				if(!bPack)
				{	
					map <string,PACKRULEFILE>::iterator itorPackRuleFile = CGlobalData::m_mapPackRuleFileInfo.find(pFindName);
					if(itorPackRuleFile != CGlobalData::m_mapPackRuleFileInfo.end())
					{
						itorPackRuleFile->second.bSelect = 1;
					}		
				}
				if(!bWeb)
				{
					map <string,LoginFileContentInfo>::iterator itorLoginFileInfo = CGlobalData::m_mapHttpFileInfo.find(pFindName);
					if(itorLoginFileInfo != CGlobalData::m_mapHttpFileInfo.end())
					{
						itorLoginFileInfo->second.bSelect = 1;
					}

				}
				memset(pFindName,0,sizeof(pFindName));
				strItemName.Empty();
			}
		}

		if(!bPack)
			m_packThread.OpenThread(&m_PackServiceType);	
	    if(!bWeb)
			m_webThread.OpenThread();
		if(!bMatch)
			m_matchThread.OpenThread();
	}
	else
	{	
		map <string,PACKRULEFILE>::iterator itorPackRule = CGlobalData::m_mapPackRuleFileInfo.begin();
		for(itorPackRule;itorPackRule != CGlobalData::m_mapPackRuleFileInfo.end();++itorPackRule)
		{
			itorPackRule->second.bSelect = 1;
		}
		if(!bPack)
			m_packThread.OpenThread(&m_PackServiceType);	
	}

	GetDlgItem(ID_BUTTON_TEST)->EnableWindow(FALSE);
	GetDlgItem(ID_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(ID_BUTTON_PACKINFO)->EnableWindow(FALSE);
	GetDlgItem(ID_BUTTONG_SET)->EnableWindow(FALSE);
}

void CLoginTestDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonStop();
	OnCancel();
}

void CLoginTestDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	map <string,PACKRULEFILE>::iterator itorPackRule = CGlobalData::m_mapPackRuleFileInfo.begin();
	for(itorPackRule;itorPackRule != CGlobalData::m_mapPackRuleFileInfo.end();++itorPackRule)
	{
		itorPackRule->second.bSelect = 0;
	}

	map <string,LoginFileContentInfo>::iterator itorLoginFileInfo = CGlobalData::m_mapHttpFileInfo.begin();
	for(itorLoginFileInfo;itorLoginFileInfo != CGlobalData::m_mapHttpFileInfo.end();++itorLoginFileInfo)
	{
		itorLoginFileInfo->second.bSelect = 0;
	}

	if(m_webThread.m_bRun)      
		m_webThread.Close();
	if(m_packThread.m_bRun)    
		m_packThread.Close();
	if(m_matchThread.m_bRun)	
		m_matchThread.Close();

	m_packThread.GetAdapterInfo();
	GetDlgItem(ID_BUTTON_TEST)->EnableWindow(FALSE);
	GetDlgItem(ID_BUTTON_PACKINFO)->EnableWindow(TRUE);
	GetDlgItem(ID_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(ID_BUTTONG_SET)->EnableWindow(TRUE);

	((CButton *)GetDlgItem(IDC_CHECK_CLEAR_INTERNET))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_CHECK_CLEAR_COOKIE))->SetCheck(FALSE);
}

void __stdcall CLoginTestDlg::_CallStopCapture()
{
	map <string,PACKRULEFILE>::iterator itorPackRule = CGlobalData::m_mapPackRuleFileInfo.begin();
	for(itorPackRule;itorPackRule != CGlobalData::m_mapPackRuleFileInfo.end();++itorPackRule)
	{
		itorPackRule->second.bSelect = 0;
	}

	if(m_pThis->m_packThread.m_bRun)    
		m_pThis->m_packThread.Close();

	m_pThis->m_packThread.GetAdapterInfo();
	m_pThis->GetDlgItem(ID_BUTTON_TEST)->EnableWindow(FALSE);
	m_pThis->GetDlgItem(ID_BUTTON_PACKINFO)->EnableWindow(TRUE);
	m_pThis->GetDlgItem(ID_BUTTON_STOP)->EnableWindow(FALSE);
	m_pThis->GetDlgItem(ID_BUTTONG_SET)->EnableWindow(TRUE);

	((CButton *)m_pThis->GetDlgItem(IDC_CHECK_CLEAR_INTERNET))->SetCheck(FALSE);
	((CButton *)m_pThis->GetDlgItem(IDC_CHECK_CLEAR_COOKIE))->SetCheck(FALSE);
}

void __stdcall CLoginTestDlg::UpdatePackState(char *pItemName)
{
	m_pThis->FindItemAndAdd(pItemName,3);
}

void __stdcall CLoginTestDlg::UpdataWebState(char *pItemName)
{
	m_pThis->FindItemAndAdd(pItemName,2);
}

void __stdcall CLoginTestDlg::UpdataMatchState(char *pItemName)
{
	m_pThis->FindItemAndAdd(pItemName,4);
}
void CLoginTestDlg::FindItemAndAdd(char *ItemName,int nNum)
{
	CString strItemName(ItemName);
	CString strGetItem;
	int i = 0;
	for(i = 0;i<m_ListCtrl.GetItemCount();i++)
	{	
		strGetItem = m_ListCtrl.GetItemText(i,1);
		if(strItemName == strGetItem)
		{
			m_pThis->m_ListCtrl.SetItemText(i,nNum,_T("OK"));
			return;
		}
	}
}

void CLoginTestDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码 
	int i;
	if(m_bSelectState)
	{
		for(i = 0;i<m_ListCtrl.GetItemCount();i++)
		{
			m_ListCtrl.SetCheck(i,TRUE);
		}
		GetDlgItem(IDC_BUTTON_SELECT)->SetWindowText(_T("取消全选"));
		m_bSelectState = FALSE;
	}
	else
	{
		for(i = 0;i<m_ListCtrl.GetItemCount();i++)
		{
			m_ListCtrl.SetCheck(i,FALSE);
		}
		GetDlgItem(IDC_BUTTON_SELECT)->SetWindowText(_T("全  选"));
		m_bSelectState = TRUE;
	}
}

BOOL CLoginTestDlg::DeleteUrlCache(DEL_CACHE_TYPE type)
{
	BOOL bRet = FALSE;
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
	DWORD dwEntrySize;

	dwEntrySize = 0;
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
	if (!hEntry)
	{
		goto cleanup;
	}

	do
	{
		if (type == emFile &&
			!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else if (type == emCookie &&
			(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}

		dwEntrySize = 0;
		FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
		delete [] lpCacheEntry; 
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	}
	while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));

	bRet = TRUE;
cleanup:
	if (lpCacheEntry)
	{
		delete [] lpCacheEntry; 
	}
	return bRet;
}

BOOL CLoginTestDlg::EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni,BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += _T("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 || 
			_tcscmp(wfd.cFileName, _T("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, _T("desktop.ini")) == 0))
		{
			continue;
		}

		sFullPath = szPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;

		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else
		{
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, _T("index.dat")) == 0)
			{
				WipeFile(szPath, wfd.cFileName);
			}
			DeleteFile(sFullPath);
		}
	}
	while (FindNextFile(hFind, &wfd));
	FindClose(hFind);

	return TRUE;
}

BOOL CLoginTestDlg::WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	char	sZero[SWEEP_BUFFER_SIZE];
	memset(sZero, 0, SWEEP_BUFFER_SIZE);

	sPath = szDir;
	sPath += _T('\\');
	sPath += szFile;

	hFile = CreateFile(sPath, GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	dwSize = GetFileSize(hFile, NULL);

	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);

	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(hFile, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}
	CloseHandle(hFile);
	return TRUE;
}

void CLoginTestDlg::OnBnClickedButtonShowCapturemode()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = 0;
	m_ListCtrl.DeleteAllItems();
	if(m_bShowMode)
	{
		map<string,LoginFileContentInfo>::iterator pInterInfo = CGlobalData::m_mapHttpFileInfo.begin();
		for(pInterInfo;pInterInfo != CGlobalData::m_mapHttpFileInfo.end();++pInterInfo)
		{
			AddListInfo((pInterInfo->second.Item),nRow);
			nRow++;
		}
		GetDlgItem(IDC_BUTTON_SHOW_CAPTUREMODE)->SetWindowText(_T("切换到Pack规则显示"));
		m_bShowMode = FALSE;
	}
	else
	{	
		map<string,PACKRULEFILE>::iterator pPackRule = CGlobalData::m_mapPackRuleFileInfo.begin();
		for(pPackRule;pPackRule != CGlobalData::m_mapPackRuleFileInfo.end();++pPackRule)
		{
			AddListInfo((pPackRule->second.Item),nRow);
			nRow++;
		}
		GetDlgItem(IDC_BUTTON_SHOW_CAPTUREMODE)->SetWindowText(_T("切换到Web规则显示"));
		m_bShowMode = TRUE;
	}
}
