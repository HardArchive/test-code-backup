// DownloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "DownloadDlg.h"

// CDownloadDlg 对话框

IMPLEMENT_DYNAMIC(CDownloadDlg, CDialog)
CDownloadDlg   *CDownloadDlg::m_pThis;

CDownloadDlg::CDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadDlg::IDD, pParent)
{
	m_strRootName = "已完成";
	memset(m_strSendORRecvBuff,0,SEND_OR_RECV_LEN);
	memset(m_strPackEnd,0,sizeof(m_strPackEnd));
	memset(m_strSaveFilePath,0,MAX_PATH);
	memcpy(m_strPackEnd,"|END|END",strlen("|END|END"));             //Xt: 包的结束标志
	m_nPackLen = strlen(m_strPackEnd);                              //Xt: 包的结束标志长度
	m_hRecvAndSendEvent = CreateEvent(NULL,TRUE,FALSE,NULL);        //Xt: 收发事件
	m_bRecvAndSendEvent = FALSE;
	m_hRecvFileThread   = NULL;
	m_pThis = this;
} 

CDownloadDlg::~CDownloadDlg()
{
	CloseResources();
}

void CDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SHOW_DOWNLOAD_PATH, m_treeControl);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD_STATE, m_ProgressCtrl);
}

BOOL CDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	if(!LinkService(CGlobalData::m_strServiceIP,CGlobalData::m_nServicePort))
	{
		CGlobalData::PrintLogInfo("CDownloadDlg","连接服务程序错误或服务程序没有启动!",'E');
		MessageBox(_T("连接服务程序错误或服务器没有启动!"));
		GetDlgItem(IDC_TREE_SHOW_DOWNLOAD_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWNLOAD_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_PROGRESS_DOWNLOAD_STATE)->EnableWindow(FALSE);
	}

	m_hTreeRoot     = m_treeControl.InsertItem(_T("远程下载目录..."),TVI_ROOT,TVI_LAST);
	m_hTreePathHead = m_treeControl.InsertItem(m_strRootName,m_hTreeRoot,TVI_LAST);
	ResetEvent(m_hRecvAndSendEvent);
	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDownloadDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_SAVE, &CDownloadDlg::OnBnClickedButtonDownloadSave)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_SHOW_DOWNLOAD_PATH, &CDownloadDlg::OnTvnItemexpandingTreeShowDownloadPath)
END_MESSAGE_MAP()

// CDownloadDlg 消息处理程序

void CDownloadDlg::OnBnClickedButtonDownloadSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString		strSavePath;
	CString     strSaveFileName = m_treeControl.GetItemText(m_treeControl.GetSelectedItem());
	CFileDialog	fDlg(FALSE,NULL,strSaveFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		_T("RAR FILE(*.RAR)|*.RAR|| "),NULL);
	if(fDlg.DoModal() == IDOK)
	{
		strSavePath = fDlg.GetPathName();
 		CChangeCode::UnicodeToGB2312Code(m_strSaveFilePath,strSavePath.GetBuffer());
		if(strSavePath.IsEmpty())
		{
			MessageBox(_T("保存路径错误!"));
			return;
		}
		else
		{
			m_strShowMSG.Format(_T("文件\"%s\" 保存到\"%s\"路径 "),strSaveFileName.GetBuffer(),strSavePath.GetBuffer());
			GetDlgItem(IDC_STATIC_SHOW_MSG)->SetWindowText(m_strShowMSG);

			int nPackLen = 0;
			char strSendFilePath[MAX_PATH] = {0};
			if(GetCheckTreeNodePath(strSendFilePath))
 			{
				Package(m_strSendORRecvBuff,'D',strSendFilePath,nPackLen);		                 //Xt: 封包
				SendData(m_strSendORRecvBuff,nPackLen);
		    }
		}
	}
}

void CDownloadDlg::OnTvnItemexpandingTreeShowDownloadPath(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
	m_hTree	 = hTreeItem;                                                                //Xt: 当前点击的路径                                    
	m_treeControl.SelectItem(hTreeItem);                                                 //Xt: 树控件选中节点
                
	if(hTreeItem == m_hTreeRoot)
	{
		HTREEITEM hChileTree = m_treeControl.GetChildItem(m_hTreePathHead);				 //Xt: 得到当前选中的子节点
		if(NULL == hChileTree)
		{
			int     nPackLen = 0;
			char    strRootName[MAX_PATH] = {0};
			CChangeCode::UnicodeToGB2312Code(strRootName,m_strRootName.GetBuffer());
			Package(m_strSendORRecvBuff,'M',strRootName,nPackLen);		                 //Xt: 封包
			SendData(m_strSendORRecvBuff,nPackLen);
			m_treeControl.EnableWindow(FALSE);
		}
	}
 	*pResult = 0;
}

BOOL CDownloadDlg::Package(char *pPack,char cType,char *pSendData,int &nSendDataLen)
{
	if(cType == '\0' || NULL == pPack || NULL == pSendData)
	{
		return false;
	}
	memset(pPack,0,SEND_OR_RECV_LEN);
	int nDataHeadLen   = sizeof(DataPackHead);
	pDataPackHead pDH  = (pDataPackHead)pPack;
	pDH->cType         = cType;
	pDH->nContentLen   = strlen(pSendData)+1;                             //Xt: 包内容长度
	pDH->nPackLen      = nDataHeadLen+pDH->nContentLen;					  //Xt: 包头加内容的长度 
	memcpy(pPack+nDataHeadLen,pSendData,strlen(pSendData)+1);                     
	memcpy(pPack+pDH->nPackLen,m_strPackEnd,m_nPackLen);                  //Xt: 添加包尾结束标志
	nSendDataLen = pDH->nPackLen + m_nPackLen;                            //Xt: 计算包的大小加结束标志
	return true;
}

void CDownloadDlg::RecvData(char *pPackData)
{
	int nDataHeadLen   = sizeof(DataPackHead);
	pDataPackHead pDH  = (pDataPackHead)pPackData;
	char *pData        = pPackData+nDataHeadLen;
	
	if(pDH->nContentLen <RECV_DATA && 0 == memcmp(pData+(pDH->nContentLen),m_strPackEnd,m_nPackLen))  //Xt: 包的结束标志是否正确
	{ 
		switch(pDH->cType)
		{
		case ('M'):                                                       //Xt: 路径节点接收
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","接收服务器服务类型M",'O');
				ResolveDataPack(pData);	
				m_treeControl.EnableWindow(TRUE);
				break;
			}
		case ('D'):                                                       //Xt:  下载文件接收
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","接收服务器服务类型D",'O');
				int nFileLen = atoi(pData);
				RecvFile(nFileLen);     	
				break;
			}
		}
	}
}	

void CDownloadDlg::ResolveDataPack(char *pData)
{
	CString strRecvPath(pData);
	CString strPathList;                                                  //Xt: 一个节点和其下分支节点
	int     nFindLocaltion = 0;
	while(!strRecvPath.IsEmpty())
	{
		 if(strRecvPath.GetAt(0) == '<')
		 {
			strPathList = strRecvPath; 
			nFindLocaltion = strPathList.Find('>')+1;
			if(-1 != nFindLocaltion)
			{
				FindAddNode(strPathList.Left(nFindLocaltion-1));
				strRecvPath = strRecvPath.GetBuffer() + nFindLocaltion;
				strPathList.Empty();
			}
			else
			{
				strRecvPath.Empty();
			}
		 }
	}
}

HTREEITEM  CDownloadDlg::FindAddNode(CString strPathList)
{
	HTREEITEM hChileNode = NULL;
	CString   strDirList = strPathList.GetBuffer() + 1;
	CString   strDirName = strDirList;
	int       nLocaltion = strDirList.Find(_T("_*"));
	
	if(-1 != nLocaltion)
	{
		strDirName = strDirList.Left(nLocaltion);
		hChileNode = m_treeControl.InsertItem(strDirName,m_hTreePathHead,TVI_LAST);
		strDirList = strDirList.GetBuffer()+nLocaltion+strlen("_*");
		strDirName.Empty();

		while(!strDirList.IsEmpty())
		{
			nLocaltion = strDirList.Find(_T("||"));
			if(-1 != nLocaltion)
			{
				strDirName = strDirList.Left(nLocaltion);
				m_treeControl.InsertItem(strDirName,hChileNode,TVI_LAST);
				strDirList = strDirList.GetBuffer() + nLocaltion +strlen("||");
				strDirName.Empty();
			}
			else
			{
				strDirList.Empty();
			}
		}
	}
	return NULL;
}

BOOL CDownloadDlg::GetCheckTreeNodePath(char *pStrPath)
{	
	HTREEITEM hCurrentTreeNode = m_treeControl.GetSelectedItem();
	if(m_treeControl.ItemHasChildren(hCurrentTreeNode))
	{
		AfxMessageBox(_T("无法下载文件夹,请选择文件下载!"));
		return FALSE;
	}
	else
	{
		HTREEITEM  hChileTree = hCurrentTreeNode;
		CString    strPathName;	
		while((m_hTreePathHead != hChileTree) && (NULL != hChileTree))
		{
			strPathName.Insert(0,m_treeControl.GetItemText(hChileTree));
			strPathName.Insert(0,_T("\\"));
			hChileTree = m_treeControl.GetParentItem(hChileTree);
		}
		strPathName.Insert(0,m_strRootName);
		CChangeCode::UnicodeToGB2312Code(pStrPath,strPathName.GetBuffer());
	}
	return TRUE;
}

void CDownloadDlg::RecvFile(int nFileLen)
{
	int  nRecvFileLen    = 0;
	int  nRecvFileSumLen = 0;
	int  nRecvSize       = sizeof(m_pStrData);

	CDownloadDlg::m_pThis->m_ProgressCtrl.SetRange(0,nFileLen);
	CDownloadDlg::m_pThis->m_ProgressCtrl.SetPos(0);
	FILE *pFile = fopen(m_strSaveFilePath,"ab+");
	while(nRecvFileSumLen < nFileLen)
	{
		memset(m_pStrData,0,nRecvSize);
		nRecvFileLen = recv(m_hClientSocket,m_pStrData,nRecvSize,0);
		if(SOCKET_ERROR == nRecvFileLen || nRecvFileLen == 0)
			break;
		nRecvFileSumLen += nRecvFileLen;
		fwrite(m_pStrData,1,nRecvFileLen,pFile);
		CDownloadDlg::m_pThis->m_ProgressCtrl.SetPos(nRecvFileSumLen);
	}
	fclose(pFile);
	pFile = NULL;
	CDownloadDlg::m_pThis->m_strShowMSG += _T("下载已完成!");
	CDownloadDlg::m_pThis->GetDlgItem(IDC_STATIC_SHOW_MSG)->SetWindowText(CDownloadDlg::m_pThis->m_strShowMSG);
	CDownloadDlg::m_pThis->m_strShowMSG.Empty();
}