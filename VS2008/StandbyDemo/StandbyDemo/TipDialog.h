#pragma once


// CTipDialog �Ի���

class CTipDialog : public CDialog
{
	DECLARE_DYNAMIC(CTipDialog)

public:
	CTipDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTipDialog();

public:
	BOOL m_bFlag;
	char m_szMessage[MAX_PATH];
	void SetPromptMessage(PCHAR pszMsg);

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
