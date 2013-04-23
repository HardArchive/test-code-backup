// UpdataDownload.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "UpdataDownload.h"
#include "../RegistryOperate.h"

// CUpdataDownload �Ի���
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
	memcpy(m_strPackEnd,"|END|END",strlen("|END|END"));             //Xt: ���Ľ�����־
	m_nPackEndLen = strlen(m_strPackEnd);                           //Xt: ���Ľ�����־����
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


// CUpdataDownload ��Ϣ�������
BOOL CUpdataDownload::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(!LinkService(CGlobalData::m_strServiceIP,CGlobalData::m_nServicePort))
	{
		CGlobalData::PrintLogInfo("CUpdataDownload","���ӷ����������������û������!",'E');
		MessageBox(_T("���ӷ���������������û������!"));
	}

	int   nSendLeng = 0;
	Package(m_pStrData,'V',"",nSendLeng);
	SendData(m_pStrData,nSendLeng);

	CString strVersion(_T("�ͻ��˵�ǰ�汾��Ϊ: "));
	strVersion += CGlobalData::m_strClientVersionNum;
	GetDlgItem(IDC_STATIC_CLIENT_ID)->SetWindowText(strVersion);
	GetDlgItem(ID_BUTTON_UPDATA)->EnableWindow(FALSE);
	return TRUE;
}

void CUpdataDownload::OnBnClickedButtonUpdata()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

		//Xt: ���°汾��
		HKEY hKey;
		CRegistryOperate RegOpt;
		if(RegOpt.OpenRegistry(HKEY_LOCAL_MACHINE,"SOFTWARE\\CaptureTest",&hKey))
		{
			RegOpt.SetRegistryValue("Version",CGlobalData::m_strServerVersionNum,&hKey);
			RegCloseKey(hKey);
			memcpy(CGlobalData::m_strClientVersionNum,CGlobalData::m_strServerVersionNum,sizeof(CGlobalData::m_strClientVersionNum)); 
			
			CString strClientVersion(_T("�ͻ��˵�ǰ�汾��Ϊ: "));
		    strClientVersion += CGlobalData::m_strClientVersionNum;
			GetDlgItem(IDC_STATIC_CLIENT_ID)->SetWindowText(strClientVersion);
			GetDlgItem(ID_BUTTON_UPDATA)->EnableWindow(FALSE);
			MessageBox(_T("�����ɹ��������°汾!"));
		}
	}
	else if(WAIT_TIMEOUT == nWiatResult)
	{
		m_bRun = false;
		CGlobalData::PrintLogInfo("CUpdataDownload","����ʧ������ʱ�䳬ʱ!",'E');
		MessageBox(_T("����ʧ������ʱ�䳬ʱ!"));
	}
}

void  CUpdataDownload::RecvData(char *pPackData)
{
	int nDataHeadLen   = sizeof(DataPackHead);
	pDataPackHead pDH  = (pDataPackHead)pPackData;
	char *pData        = pPackData+nDataHeadLen;

	if(pDH->nContentLen <RECV_DATA && 0 == memcmp(pData+(pDH->nContentLen),m_strPackEnd,m_nPackEndLen))  //Xt: ���Ľ�����־�Ƿ���ȷ
	{ 
		switch(pDH->cType)
		{
		case ('V'):                                                                                      //Xt: ·���ڵ����
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","���շ�������������V",'O');
				int nRecvContentLen = strlen(pData);
				if(pData[0] != '\0' && nRecvContentLen<10)
				{
					memcpy(CGlobalData::m_strServerVersionNum,pData,nRecvContentLen);
					CString strVersion(_T("��������ǰ�汾��Ϊ: "));
					strVersion += CGlobalData::m_strServerVersionNum;
					CUpdataDownload::m_pThis->GetDlgItem(IDC_STATIC_SERVER_ID)->SetWindowText(strVersion);
					//Xt: ��⵱ǰ�汾�ͷ������汾�Ƿ�һ��
					if(memcmp(CGlobalData::m_strClientVersionNum,CGlobalData::m_strServerVersionNum,strlen(CGlobalData::m_strClientVersionNum)))
					{
						GetDlgItem(ID_BUTTON_UPDATA)->EnableWindow(TRUE);
					}
				}
				break;
			}
		case ('U'):                                                       //Xt:  �����ļ�����
			{
				CGlobalData::PrintLogInfo("CDownloadDlg","���շ�������������U",'O');
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
	strBackupFilePath.insert(nPos,"Backup");                             //Xt:  �����ļ�����

	FILE *pFile = fopen(pFilePath,"rb");                                 //Xt:  ���������ļ����浽����λ�ñ���
	if(NULL == pFile)
	{
		return false;	
	}
	fclose(pFile);
	rename(strFilePath.c_str(),strBackupFilePath.c_str());

	int  nRecvFilePost = 0;												 //Xt:  ��ǰ�����ļ��ĳ���
	int  nRecvSize     = sizeof(m_pStrData);
	FILE *pSaveFile = fopen(pFilePath,"ab+");
	if(NULL != pSaveFile)
	{	                                        
		while((nRecvFilePost < nRecvFileLen) && m_bRun)                  //Xt:  �ж����ļ������Ƿ��Ѿ�������
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
	pDH->nContentLen   = strlen(pSendData)+1;                             //Xt: �����ݳ���
	pDH->nPackLen      = nDataHeadLen+pDH->nContentLen;					  //Xt: ��ͷ�����ݵĳ��� 
	memcpy(pPack+nDataHeadLen,pSendData,strlen(pSendData)+1);                     
	memcpy(pPack+pDH->nPackLen,m_strPackEnd,m_nPackEndLen);               //Xt: ��Ӱ�β������־
	nSendDataLen = pDH->nPackLen + m_nPackEndLen;                         //Xt: ������Ĵ�С�ӽ�����־
	return TRUE;
}