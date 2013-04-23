#pragma once
//#include "afxcmn.h"
#include "../ClientSocket.h"
#include "afxcmn.h"

#define		SEND_OR_RECV_LEN	1024
// CDownloadDlg 对话框
class CDownloadDlg : public CDialog,CClientSocket
{
	DECLARE_DYNAMIC(CDownloadDlg)

public:
	CDownloadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadDlg();
	virtual BOOL    OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void    DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void    OnBnClickedButtonDownloadSave();
	afx_msg void    OnTvnItemexpandingTreeShowDownloadPath(NMHDR *pNMHDR, LRESULT *pResult);
public:   
	BOOL			Package(char *pPack,char cType,char *pSendData,int &nSendDataLen);    //Xt: 封包
	void			ResolveDataPack(char *pData);                                         //Xt: 接收路径包解析
	BOOL            GetCheckTreeNodePath(char *pStrPath);                                 //Xt: 得到当前下载的树文件路径
	HTREEITEM		FindAddNode(CString strPathList);                                     //Xt: 通过路径名查找返回树节点
	virtual	void	RecvData(char *pData); 
	void            RecvFile(int  nFileLen);                                         //Xt: 接收文件线程
public:
	int             m_nPackLen;                                                     
	char            m_strPackEnd[10];                                      //Xt:  包结束符
	char            m_strSendORRecvBuff[SEND_OR_RECV_LEN];                 //Xt:  发送BUFF
	char            m_strSaveFilePath[MAX_PATH];                           //Xt:  保存下载的路径名
	CString         m_strRootName;                                         //Xt:  树根节点名称
	CString         m_strShowMSG;
	
	HTREEITEM		m_hTree;                                               //Xt:  当前树的节点
	HTREEITEM       m_hTreeRoot;                                           //Xt:  路径头节点
	HTREEITEM       m_hTreePathHead;                                       //Xt:  树的路径头节点
	HANDLE          m_hRecvAndSendEvent;                                   //Xt:  收发开关(收到之后再发送)
	HANDLE          m_hRecvFileThread;                                     //Xt:  接收文件线程句柄
	volatile BOOL   m_bRecvAndSendEvent;    

	CProgressCtrl   m_ProgressCtrl;
    CTreeCtrl		m_treeControl;
public:
	static CDownloadDlg   *m_pThis;
};
