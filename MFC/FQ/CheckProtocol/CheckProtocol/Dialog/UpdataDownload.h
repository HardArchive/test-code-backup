#pragma once
#include "../ClientSocket.h"

// CUpdataDownload 对话框

class CUpdataDownload : public CDialog,public CClientSocket
{
	DECLARE_DYNAMIC(CUpdataDownload)

public:
	CUpdataDownload(CWnd* pParent = NULL);                                       // 标准构造函数
	virtual ~CUpdataDownload();
// 对话框数据
	enum { IDD = IDD_DIALOG_UPDATA_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);                             // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual	void  RecvData(char *pData);
	afx_msg void OnBnClickedButtonUpdata();

public:
	bool    RecvUpadatFile(char *pFilePath,int nRecvFileLen);
	bool    UpdataFile(char *pUpdataFileName,int nUpDataFileLen);                //Xt: 更新升级文件
	BOOL	Package(char *pPack,char cType,char *pSendData,int &nSendDataLen);   //Xt: 封包
public:
	HANDLE         m_hEvent;
	volatile bool  m_bRun;
	static   char  m_strDownloadName[MAX_PATH];
	static 	 CUpdataDownload *m_pThis;      
};
