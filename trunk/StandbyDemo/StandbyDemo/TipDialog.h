#pragma once


// CTipDialog 对话框

class CTipDialog : public CDialog
{
	DECLARE_DYNAMIC(CTipDialog)

public:
	CTipDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTipDialog();

public:
	BOOL m_bFlag;
	char m_szMessage[MAX_PATH];
	void SetPromptMessage(PCHAR pszMsg);

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
protected:
	virtual void PreInitDialog();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
