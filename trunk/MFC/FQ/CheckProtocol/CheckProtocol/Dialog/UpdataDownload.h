#pragma once
#include "../ClientSocket.h"

// CUpdataDownload �Ի���

class CUpdataDownload : public CDialog,public CClientSocket
{
	DECLARE_DYNAMIC(CUpdataDownload)

public:
	CUpdataDownload(CWnd* pParent = NULL);                                       // ��׼���캯��
	virtual ~CUpdataDownload();
// �Ի�������
	enum { IDD = IDD_DIALOG_UPDATA_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);                             // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual	void  RecvData(char *pData);
	afx_msg void OnBnClickedButtonUpdata();

public:
	bool    RecvUpadatFile(char *pFilePath,int nRecvFileLen);
	bool    UpdataFile(char *pUpdataFileName,int nUpDataFileLen);                //Xt: ���������ļ�
	BOOL	Package(char *pPack,char cType,char *pSendData,int &nSendDataLen);   //Xt: ���
public:
	HANDLE         m_hEvent;
	volatile bool  m_bRun;
	static   char  m_strDownloadName[MAX_PATH];
	static 	 CUpdataDownload *m_pThis;      
};
