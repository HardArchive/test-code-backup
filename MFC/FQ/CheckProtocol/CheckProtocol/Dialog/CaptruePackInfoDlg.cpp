// CaptruePackInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "CaptruePackInfoDlg.h"
#include "../Thread/PackThread.h"

CCaptruePackInfoDlg *CCaptruePackInfoDlg::m_pThis = NULL;
// CCaptruePackInfo 对话框

IMPLEMENT_DYNAMIC(CCaptruePackInfoDlg, CDialog)
CCaptruePackInfoDlg::CCaptruePackInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptruePackInfoDlg::IDD, pParent)
{
	m_pThis = this;
}

CCaptruePackInfoDlg::~CCaptruePackInfoDlg()
{
	m_pThis = NULL;
}

void CCaptruePackInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PACK_INFO, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CCaptruePackInfoDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CCaptruePackInfoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_BUTTON_SAVE, &CCaptruePackInfoDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CCaptruePackInfoDlg::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()


// CCaptruePackInfo 消息处理程序

BOOL CCaptruePackInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(!AddListItem())	return FALSE;

// 	WaitForSingleObject(CGlobalData::m_hMutex,INFINITE);
// 	int nRow = 0;
// 	multimap<string,pack_info *>::iterator pMulMapInterCeptInfo =  CPackThread::m_MulMapInterceptedInfo.begin();
// 	for(pMulMapInterCeptInfo;pMulMapInterCeptInfo != CPackThread::m_MulMapInterceptedInfo.end();++pMulMapInterCeptInfo)
// 	{
// 		AddListInfo(&(*(pMulMapInterCeptInfo->second)),nRow);
// 		UpdateData(FALSE);
// 	}
//	ReleaseMutex(CGlobalData::m_hMutex);
	m_pThis = this;
	LoadIntercepterInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CCaptruePackInfoDlg::AddListItem()
{
	m_ListCtrl.ShowScrollBar(SB_HORZ,FALSE);
	m_ListCtrl.ShowScrollBar(SB_VERT,TRUE);
	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES|LVS_EX_TRACKSELECT|\
		LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);

	if(-1 == m_ListCtrl.InsertColumn(0, _T("ItemName"),LVCFMT_LEFT,100))	     return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(1, _T("UserName"),LVCFMT_CENTER,100))		 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(2, _T("Password"),LVCFMT_CENTER,100))		 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(3, _T("SendContent"),LVCFMT_CENTER,100))	 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(4, _T("PackDirection"),LVCFMT_CENTER,100))  return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(5, _T("ip_Src"),LVCFMT_CENTER,100))         return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(6, _T("ip_Dst"),LVCFMT_CENTER,100))		 return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(7, _T("SrcPort"),LVCFMT_CENTER,100))        return FALSE;
	if(-1 == m_ListCtrl.InsertColumn(8, _T("DestPort"),LVCFMT_CENTER,100))       return FALSE; 
	if(-1 == m_ListCtrl.InsertColumn(9, _T("SrcMac"),LVCFMT_CENTER,50))	         return FALSE; 
// 	if(-1 == m_ListCtrl.InsertColumn(10,_T("DestMac"),LVCFMT_CENTER),50)         return FALSE;         
// 	if(-1 == m_ListCtrl.InsertColumn(11,_T("PackTime"),LVCFMT_CENTER),50) 		 return FALSE;
	return TRUE;
}

void CCaptruePackInfoDlg::AddListInfo(RADE_FILE *ReadFile,int nRow)
{
 	CString str(ReadFile->Item);
	m_pThis->m_ListCtrl.InsertItem(nRow,str);
	str = ReadFile->UserName;
	m_pThis->m_ListCtrl.SetItemText(nRow,1,str);
	str = ReadFile->Password;
	m_pThis->m_ListCtrl.SetItemText(nRow,2,str);
	str = ReadFile->SendContent;
	m_pThis->m_ListCtrl.SetItemText(nRow,3,str);
	
	str = ReadFile->PackDirection;
	m_pThis->m_ListCtrl.SetItemText(nRow,4,str);

	str = ReadFile->ip_src;
 	m_pThis->m_ListCtrl.SetItemText(nRow,5,str);

	str = ReadFile->ip_dst;
	m_pThis->m_ListCtrl.SetItemText(nRow,6,str);

	str = ReadFile->SrcPort;
    m_pThis->m_ListCtrl.SetItemText(nRow,7,str);

	str = ReadFile->DestPort;
	m_pThis->m_ListCtrl.SetItemText(nRow,8,str);

	str = ReadFile->SrcMac;
    m_pThis->m_ListCtrl.SetItemText(nRow,9,str);
	str = ReadFile->DestMac;
	m_pThis->m_ListCtrl.SetItemText(nRow,10,str);
// 	str = pPackInfo->PackTime;
// 	CCaptruePackInfoDlg::m_ListCtrl.InsertItem(nRow,str);
	//UpdateData(FALSE);
}

void __stdcall CCaptruePackInfoDlg::UpdateNotice()
{
// 	if(NULL != CCaptruePackInfoDlg::m_pThis)
// 	{
//  		CCaptruePackInfoDlg::m_ListCtrl.DeleteAllItems();
// 		int nRow = 0;
// 
// 		WaitForSingleObject(CGlobalData::m_hMutex,INFINITE);
// 		multimap<string,pack_info *>::iterator pMulMapInterCeptInfo =  CPackThread::m_MulMapInterceptedInfo.begin();
// 		for(pMulMapInterCeptInfo;pMulMapInterCeptInfo != CPackThread::m_MulMapInterceptedInfo.end();++pMulMapInterCeptInfo)
// 		{
// 			AddListInfo(&(*(pMulMapInterCeptInfo->second)),nRow);
// 			nRow++;
// 		}
// 		ReleaseMutex(CGlobalData::m_hMutex);
// 	}
	m_pThis->m_ListCtrl.DeleteAllItems();
	LoadIntercepterInfo();
}

void CCaptruePackInfoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CCaptruePackInfoDlg::LoadIntercepterInfo()
{
	char strFileContentInfo[1024] = {0};
	FILE *pFile = fopen(CGlobalData::m_strInterceptedInfoPath,"rb+");
	if(!pFile)	
	{
		//sprintf(m_strError,"\"%s\" 文件夹下无配置文件文件",m_strHttpFilePath);
		return;
	}
	else
	{	
		int nNumber = 0;	
		RADE_FILE readFile;
		while(fgets(strFileContentInfo,sizeof(strFileContentInfo),pFile))
		{
			if(*strFileContentInfo == EOF || *strFileContentInfo == '\0')
				break;
			if(CGlobalData::FindInterceptedFile(strFileContentInfo,&readFile))
			{
				AddListInfo(&readFile,nNumber);
				memset(&readFile,0,sizeof(readFile));
				nNumber++;
			}
		}
	}
	fclose(pFile);
}

void CCaptruePackInfoDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilePath;
	CString strError;
	CFileDialog  fileDlg(FALSE,_T(".txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
					     _T("TXT FILE(*.TXT)|*.TXT||"),NULL);
	if(fileDlg.DoModal() == IDOK)
	{
		strFilePath.Format(_T("%s"),fileDlg.GetPathName());
	}

	CFileFind fFind;
	CString strSrcFilePath;
	strSrcFilePath = (CGlobalData::m_strInterceptedInfoPath);

	FILE *file = fopen(CGlobalData::m_strInterceptedInfoPath,"r");
	if(file)                                                                             //Xt:判断目录是否存在	
	{
		fclose(file);
		if(!MoveFile(strSrcFilePath,strFilePath))                                        //Xt：移动文件夹
		{
			strError.Format(_T("保存文件错误!\r\n%d"),GetLastError());
		}
	}
}

void CCaptruePackInfoDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
}
