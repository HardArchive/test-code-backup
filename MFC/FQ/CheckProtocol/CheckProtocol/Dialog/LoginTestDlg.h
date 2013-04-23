#pragma once
#include <wininet.h>                                     //网页清除操作包含的头文件
#include "afxcmn.h"
#include "afxwin.h"

#include "../Thread/MatchThread.h"
#include "../Thread/PackThread.h"
#include "../Thread/WebThread.h"

#define SWEEP_BUFFER_SIZE			                     10000
// CLoginTestDlg 对话框

class CLoginTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginTestDlg)

public:
	CLoginTestDlg(CWnd* pParent = NULL);                // 标准构造函数
	virtual ~CLoginTestDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOGINTEST };
	enum DEL_CACHE_TYPE                                 //要删除的类型。
	{
		emFile,                                         //表示internet临时文件
		emCookie                                        //表示Cookie
	};  
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPackinfo();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtongSet();
	afx_msg void OnBnClickedButtonStop();
public:
	BOOL AddListItem();
	void AddListInfo(char *pItemName,int nRow); 
	void FindItemAndAdd(char *ItemName,int nNum);
	static CLoginTestDlg **GetInstance()
	{
		return &m_pThis;
	}
	//清理IE信息
    BOOL DeleteUrlCache(DEL_CACHE_TYPE type);
	BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE,BOOL bWipeIndexDat = FALSE); 
	BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile);
public:
	void static     __stdcall UpdatePackState(char *pItemName);
	void static     __stdcall UpdataWebState(char *pItemName);
	void static     __stdcall UpdataMatchState(char *pItemName);
	void static     __stdcall _CallStopCapture();
	afx_msg void    OnBnClickedButtonSelect();
	afx_msg void    OnBnClickedButtonShowCapturemode();
public:
	CListCtrl              m_ListCtrl;	
	CPackThread            m_packThread;
	CWebThread		       m_webThread;
	CMatchThread           m_matchThread;
	CComboBox              m_ComboBox;	
	static CLoginTestDlg  *m_pThis;
private:
	BOOL                   m_bSelectState;
	BOOL                   m_bShowMode;
	int                    m_PackServiceType;
	CString                m_strError;
};
