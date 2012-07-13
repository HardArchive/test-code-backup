
// MySqlDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "NetMySql.h"
#include "afxcmn.h"

// CMySqlDemoDlg �Ի���
class CMySqlDemoDlg : public CDialog
{
// ����
public:
	CMySqlDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYSQLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	CNetMySql m_objMySql;

public:
	BOOL bRun;
	void InitMySql();


// ʵ��
protected:
	HICON m_hIcon;
	CDC      m_objCDC;                                            //����һ�ݴ�DC md
	CBitmap  m_objBitmap;       
	void LoadPathImage(PCHAR pszPath);                           //��·������ͼƬ

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListLeadShip;
	afx_msg void OnBnClickedButtonTest();
	CHotKeyCtrl m_HotKeyCtrl;
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	CHotKeyCtrl m_HotKeyCtrlSHOW;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
