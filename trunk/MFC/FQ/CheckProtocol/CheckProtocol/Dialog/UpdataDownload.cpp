// UpdataDownload.cpp : 实现文件
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "UpdataDownload.h"
#include "../RegistryOperate.h"

// CUpdataDownload 对话框
IMPLEMENT_DYNAMIC(CUpdataDownload, CDialog)

CUpdataDownload *CUpdataDownload::m_pThis = NULL;
char  CUpdataDownload::m_strDownloadName[MAX_PATH] = {0};

CUpdataDownload::CUpdataDownload(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdataDownload::IDD, pParent)
{
	m_pThis  = this;
	m_hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_bRun   = FALSE;
	ResetEvent(m_hEvent);
	memset(m_strPackEnd,0,sizeof(m_strPackEnd));
	memcpy(m_strPackEnd,"|END|END",strlen("|END|END"));             //Xt: 包的结束标志
	m_nPackEndLen = strlen(m_strPackEnd);                           //Xt: 包的结束标志长度
}

CUpdataDownload::~CUpdataDownload()
{
	m_bRun = false;
	if(m_hEvent)
	{
		CloseHandle(m_hEvent);
	}
	CloseResources();
}

void CUpdataDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdataDownload, CDialog)
	ON_BN_CLICKED(ID_BUTTON_UPDATA, &CUpdataDownload::OnBnClickedButtonUpdata)
END_MESSAGE_MAP()


// CUpdataDownload 消息处理程序
BOOL CUpdataDownload::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(!LinkService(CGlobalData::m_strServiceIP,CGlobalData::m_nServicePort))
	{
		CGlobalData::PrintLogInfo("CUpdataDownload","连接服务程序错误或服务程序没有启动!",'E');
		MessageBox(_T("连接服务程序错误或服务器没有启动!"));
	}

	int   nSendLeng = 0;
	Package(m_pStrData,'V',"",nSendLeng);
	SendData(m_pStrData,nSendLeng);

	CString strVersion(_T("客户端当前版本号为: "));
	strVersion += CGlobalData::m_strClientVersionNum;
	GetDlgItem(IDC_STATIC_CLIENT_ID)->SetWindowText(strVersion);
	GetDlgItem(ID_BUTTON_UPDATA)->EnableWindow(FALSE);
	return TRUE;
}

void CUpdataDownload::OnBnClickedButtonUpdata()
{
	// TODO: 在此添加控件通知处理程序代码
	int		nSendLeng = 0;
	int     nPos      = 0;
	string  strUpataFileName(CGlobalData::m_strHttpFilePath);
	nPos =  strUpataFileName.find_last_of('\\');
	memcpy(m_strDownloadName,strUpataFileName.c_str()+(nPos+1),strUpataFileName.length()-nPos);

	Package(m_pStrData,'U',m_strDownloadName,nSendLeng);
	SendData(m_pStrData,nSendLeng);
    
	int  nWiatResult = WaitForSingleObject(m_hEvent,15*1000);
	if(WAIT_OBJECT_0 == nWiatResult)
	{
		memset(m_strDownloadName,0,sizeof(m_strDownloadName));
		strUpataFileName.empty();
		nPos = 0;
		strUpataFileName = CGlobalData::m_strPackRulePath;
		nPos =  strUpataFileName.find_last_of('\\');
		memcpy(m_strDownloadName,strUpataFileName.c_str()+(nPos+1),strUpataFileName.length()-nPos);
		Package(m_pStrData,'U',m_strDownloadName,nSendLeng);
		SendData(m_pStrData,nSendLeng);

		//Xt: 更新版本号
		HKEY hKey;
		CRegistryOperate RegOpt;
		if(RegOpt.OpenRegistry(HKEY_LOCAL_MACHINE,"SOFTWARE\\CaptureTest",&hKey))
		{
			RegOpt.SetRegistryValue("Version",CGlobalData::m_strServerVersionNum,&hKey);
			RegCloseKey(hKey);
			memcpy(CGlobalData::m_strClientVersionNum,CGlobalData::m_strServerVersionNum,sizeof(CGlobalData::m_strClientVersionNum)); 
			
			CString strClientVersion(_T("客户端当前版本号为: "));
		    strClientVersion += CGlobalData::m_strClientVersionNum;
			GetDlgItem(IDC_STATIC_CLIENT_ID)->SetWindowText(strClientVersion);
			GetDlgItem(ID_BUTTON_UPDATA)->EnableWindow(FALSE);
			MessageBox(_T("升级成功已是最新版本!"));
		}
	}
	else if(WAIT_TIMEOUT == nWiatResult)
	{
		m_bRun = false;
		CGlobalData::PrintLogInfo("CUpdataDownload","更新失败下载时间超时!",'E');
		MessageBox(_T("更新失败下载时间超时!"));
	}
}

void  CUpdataDownload::RecvData(char *pPackData)
{
	int nDataHeadLen   = sizeof(DataPackHead);
	pDataPackHead pDH  = (pDataPackHead)pPackData;
	char *pData        = pPackData+nDataHeadLen;

	if(pDH->nContentLen <RECV_DATA && 0 == memcmp(pData+(pDH->nContentLen),m_strPackEnd,m_nPackEndLen))  //Xt: 包的结束标志是否正确
	{ 
		switch(pDH->cType)
		{
		case ('V'):                                                                                      //Xt: 路径节点接收
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","接收服务器服务类型V",'O');
				int nRecvContentLen = strlen(pData);
				if(pData[0] != '\0' && nRecvContentLen<10)
				{
					memcpy(CGlobalData::m_strServerVersionNum,pData,nRecvContentLen);
					CString strVersion(_T("服务器当前版本号为: "));
					strVersion += CGlobalData::m_strServerVersionNum;
					CUpdataDownload::m_pThis->GetDlgItem(IDC_STATIC_SERVER_ID)->SetWindowText(strVersion);
					//Xt: 检测当前版本和服务器版本是否一致
					if(memcmp(CGlobalData::m_strClientVersionNum,CGlobalData::m_strServerVersionNum,strlen(CGlobalData::m_strClientVersionNum)))
					{
						GetDlgItem(ID_BUTTON_UPDATA)->EnableWindow(TRUE);
					}
				}
				break;
			}
		case ('U'):                                                       //Xt:  下载文件接收
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","接收服务器服务类型U",'O');
				int nPackLen = atoi(pData);
				m_bRun = true;
				ResetEvent(m_hEvent);
				UpdataFile(m_strDownloadName,nPackLen);
				m_bRun = false;
				break;
			}
		}
	}	
}

bool  CUpdataDownload::UpdataFile(char *pUpdataFileName,int nUpDataFileLen)
{
	string strHttpPath(CGlobalData::m_strHttpFilePath);
	
	if(string::npos != strHttpPath.find(pUpdataFileName,0))
	{
		RecvUpadatFile(CGlobalData::m_strHttpFilePath,nUpDataFileLen);
		return true;
	}
	
	string strPackPath(CGlobalData::m_strPackRulePath);
	if(string::npos != strPackPath.find(pUpdataFileName,0))
	{
		RecvUpadatFile(CGlobalData::m_strPackRulePath,nUpDataFileLen);
		return true;
	}
	return false;
}

bool  CUpdataDownload::RecvUpadatFile(char *pFilePath,int nRecvFileLen)
{
	string strBackupFilePath(pFilePath);
	string strFilePath(pFilePath);

	int     nPos = strBackupFilePath.find('.');
	strBackupFilePath.insert(nPos,"Backup");                             //Xt:  备份文件更名

	FILE *pFile = fopen(pFilePath,"rb");                                 //Xt:  打开升级的文件保存到其他位置备份
	if(NULL == pFile)
	{
		return false;	
	}
	fclose(pFile);
	rename(strFilePath.c_str(),strBackupFilePath.c_str());

	int  nRecvFilePost = 0;												 //Xt:  当前接收文件的长度
	int  nRecvSize     = sizeof(m_pStrData);
	FILE *pSaveFile = fopen(pFilePath,"ab+");
	if(NULL != pSaveFile)
	{	                                        
		while((nRecvFilePost < nRecvFileLen) && m_bRun)                  //Xt:  判断是文件长度是否已经接收完
		{
			memset(m_pStrData,0,nRecvSize);
			nRecvFileLen = recv(m_hClientSocket,m_pStrData,nRecvSize,0);
			if(SOCKET_ERROR == nRecvFileLen || nRecvFileLen == 0)
				break;
			nRecvFilePost += nRecvFileLen;
			fwrite(m_pStrData,1,nRecvFileLen,pSaveFile);
		}
	}
	fclose(pSaveFile);
	remove(strBackupFilePath.c_str());
	SetEvent(m_hEvent);
	return false;
}

BOOL  CUpdataDownload::Package(char *pPack,char cType,char *pSendData,int &nSendDataLen)
{
	if(cType == '\0' || NULL == pPack || NULL == pSendData)
	{
		return false;
	}
	memset(pPack,0,RECV_DATA);
	int nDataHeadLen   = sizeof(DataPackHead);
	pDataPackHead pDH  = (pDataPackHead)pPack;
	pDH->cType         = cType;
	pDH->nContentLen   = strlen(pSendData)+1;                             //Xt: 包内容长度
	pDH->nPackLen      = nDataHeadLen+pDH->nContentLen;					  //Xt: 包头加内容的长度 
	memcpy(pPack+nDataHeadLen,pSendData,strlen(pSendData)+1);                     
	memcpy(pPack+pDH->nPackLen,m_strPackEnd,m_nPackEndLen);               //Xt: 添加包尾结束标志
	nSendDataLen = pDH->nPackLen + m_nPackEndLen;                         //Xt: 计算包的大小加结束标志
	return TRUE;
}