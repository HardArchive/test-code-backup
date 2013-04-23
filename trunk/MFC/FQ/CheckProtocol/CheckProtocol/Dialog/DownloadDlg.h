#pragma once
//#include "afxcmn.h"
#include "../ClientSocket.h"
#include "afxcmn.h"

#define		SEND_OR_RECV_LEN	1024
// CDownloadDlg �Ի���
class CDownloadDlg : public CDialog,CClientSocket
{
	DECLARE_DYNAMIC(CDownloadDlg)

public:
	CDownloadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDownloadDlg();
	virtual BOOL    OnInitDialog();
// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOAD };

protected:
	virtual void    DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void    OnBnClickedButtonDownloadSave();
	afx_msg void    OnTvnItemexpandingTreeShowDownloadPath(NMHDR *pNMHDR, LRESULT *pResult);
public:   
	BOOL			Package(char *pPack,char cType,char *pSendData,int &nSendDataLen);    //Xt: ���
	void			ResolveDataPack(char *pData);                                         //Xt: ����·��������
	BOOL            GetCheckTreeNodePath(char *pStrPath);                                 //Xt: �õ���ǰ���ص����ļ�·��
	HTREEITEM		FindAddNode(CString strPathList);                                     //Xt: ͨ��·�������ҷ������ڵ�
	virtual	void	RecvData(char *pData); 
	void            RecvFile(int  nFileLen);                                         //Xt: �����ļ��߳�
public:
	int             m_nPackLen;                                                     
	char            m_strPackEnd[10];                                      //Xt:  ��������
	char            m_strSendORRecvBuff[SEND_OR_RECV_LEN];                 //Xt:  ����BUFF
	char            m_strSaveFilePath[MAX_PATH];                           //Xt:  �������ص�·����
	CString         m_strRootName;                                         //Xt:  �����ڵ�����
	CString         m_strShowMSG;
	
	HTREEITEM		m_hTree;                                               //Xt:  ��ǰ���Ľڵ�
	HTREEITEM       m_hTreeRoot;                                           //Xt:  ·��ͷ�ڵ�
	HTREEITEM       m_hTreePathHead;                                       //Xt:  ����·��ͷ�ڵ�
	HANDLE          m_hRecvAndSendEvent;                                   //Xt:  �շ�����(�յ�֮���ٷ���)
	HANDLE          m_hRecvFileThread;                                     //Xt:  �����ļ��߳̾��
	volatile BOOL   m_bRecvAndSendEvent;    

	CProgressCtrl   m_ProgressCtrl;
    CTreeCtrl		m_treeControl;
public:
	static CDownloadDlg   *m_pThis;
};
