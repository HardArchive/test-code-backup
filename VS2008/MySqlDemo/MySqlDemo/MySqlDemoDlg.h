
// MySqlDemoDlg.h : 头文件
//

#pragma once
#include "NetMySql.h"
#include "afxcmn.h"

// CMySqlDemoDlg 对话框
class CMySqlDemoDlg : public CDialog
{
// 构造
public:
	CMySqlDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYSQLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CNetMySql m_objMySql;

public:
	BOOL bRun;
	void InitMySql();


// 实现
protected:
	HICON m_hIcon;
	CDC      m_objCDC;                                            //建立一暂存DC md
	CBitmap  m_objBitmap;       
	void LoadPathImage(PCHAR pszPath);                           //从路径加载图片

	// 生成的消息映射函数
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
