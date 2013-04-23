// DownloadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "DownloadDlg.h"

// CDownloadDlg �Ի���

IMPLEMENT_DYNAMIC(CDownloadDlg, CDialog)
CDownloadDlg   *CDownloadDlg::m_pThis;

CDownloadDlg::CDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadDlg::IDD, pParent)
{
	m_strRootName = "�����";
	memset(m_strSendORRecvBuff,0,SEND_OR_RECV_LEN);
	memset(m_strPackEnd,0,sizeof(m_strPackEnd));
	memset(m_strSaveFilePath,0,MAX_PATH);
	memcpy(m_strPackEnd,"|END|END",strlen("|END|END"));             //Xt: ���Ľ�����־
	m_nPackLen = strlen(m_strPackEnd);                              //Xt: ���Ľ�����־����
	m_hRecvAndSendEvent = CreateEvent(NULL,TRUE,FALSE,NULL);        //Xt: �շ��¼�
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(!LinkService(CGlobalData::m_strServiceIP,CGlobalData::m_nServicePort))
	{
		CGlobalData::PrintLogInfo("CDownloadDlg","���ӷ����������������û������!",'E');
		MessageBox(_T("���ӷ���������������û������!"));
		GetDlgItem(IDC_TREE_SHOW_DOWNLOAD_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWNLOAD_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_PROGRESS_DOWNLOAD_STATE)->EnableWindow(FALSE);
	}

	m_hTreeRoot     = m_treeControl.InsertItem(_T("Զ������Ŀ¼..."),TVI_ROOT,TVI_LAST);
	m_hTreePathHead = m_treeControl.InsertItem(m_strRootName,m_hTreeRoot,TVI_LAST);
	ResetEvent(m_hRecvAndSendEvent);
	return TRUE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDownloadDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_SAVE, &CDownloadDlg::OnBnClickedButtonDownloadSave)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_SHOW_DOWNLOAD_PATH, &CDownloadDlg::OnTvnItemexpandingTreeShowDownloadPath)
END_MESSAGE_MAP()

// CDownloadDlg ��Ϣ�������

void CDownloadDlg::OnBnClickedButtonDownloadSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox(_T("����·������!"));
			return;
		}
		else
		{
			m_strShowMSG.Format(_T("�ļ�\"%s\" ���浽\"%s\"·�� "),strSaveFileName.GetBuffer(),strSavePath.GetBuffer());
			GetDlgItem(IDC_STATIC_SHOW_MSG)->SetWindowText(m_strShowMSG);

			int nPackLen = 0;
			char strSendFilePath[MAX_PATH] = {0};
			if(GetCheckTreeNodePath(strSendFilePath))
 			{
				Package(m_strSendORRecvBuff,'D',strSendFilePath,nPackLen);		                 //Xt: ���
				SendData(m_strSendORRecvBuff,nPackLen);
		    }
		}
	}
}

void CDownloadDlg::OnTvnItemexpandingTreeShowDownloadPath(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
	m_hTree	 = hTreeItem;                                                                //Xt: ��ǰ�����·��                                    
	m_treeControl.SelectItem(hTreeItem);                                                 //Xt: ���ؼ�ѡ�нڵ�
                
	if(hTreeItem == m_hTreeRoot)
	{
		HTREEITEM hChileTree = m_treeControl.GetChildItem(m_hTreePathHead);				 //Xt: �õ���ǰѡ�е��ӽڵ�
		if(NULL == hChileTree)
		{
			int     nPackLen = 0;
			char    strRootName[MAX_PATH] = {0};
			CChangeCode::UnicodeToGB2312Code(strRootName,m_strRootName.GetBuffer());
			Package(m_strSendORRecvBuff,'M',strRootName,nPackLen);		                 //Xt: ���
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
	pDH->nContentLen   = strlen(pSendData)+1;                             //Xt: �����ݳ���
	pDH->nPackLen      = nDataHeadLen+pDH->nContentLen;					  //Xt: ��ͷ�����ݵĳ��� 
	memcpy(pPack+nDataHeadLen,pSendData,strlen(pSendData)+1);                     
	memcpy(pPack+pDH->nPackLen,m_strPackEnd,m_nPackLen);                  //Xt: ��Ӱ�β������־
	nSendDataLen = pDH->nPackLen + m_nPackLen;                            //Xt: ������Ĵ�С�ӽ�����־
	return true;
}

void CDownloadDlg::RecvData(char *pPackData)
{
	int nDataHeadLen   = sizeof(DataPackHead);
	pDataPackHead pDH  = (pDataPackHead)pPackData;
	char *pData        = pPackData+nDataHeadLen;
	
	if(pDH->nContentLen <RECV_DATA && 0 == memcmp(pData+(pDH->nContentLen),m_strPackEnd,m_nPackLen))  //Xt: ���Ľ�����־�Ƿ���ȷ
	{ 
		switch(pDH->cType)
		{
		case ('M'):                                                       //Xt: ·���ڵ����
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","���շ�������������M",'O');
				ResolveDataPack(pData);	
				m_treeControl.EnableWindow(TRUE);
				break;
			}
		case ('D'):                                                       //Xt:  �����ļ�����
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","���շ�������������D",'O');
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
	CString strPathList;                                                  //Xt: һ���ڵ�����·�֧�ڵ�
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
		AfxMessageBox(_T("�޷������ļ���,��ѡ���ļ�����!"));
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
	CDownloadDlg::m_pThis->m_strShowMSG += _T("���������!");
	CDownloadDlg::m_pThis->GetDlgItem(IDC_STATIC_SHOW_MSG)->SetWindowText(CDownloadDlg::m_pThis->m_strShowMSG);
	CDownloadDlg::m_pThis->m_strShowMSG.Empty();
}