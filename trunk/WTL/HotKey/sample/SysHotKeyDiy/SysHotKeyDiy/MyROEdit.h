#pragma once


// CMyROEdit

class CMyROEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyROEdit)

public:
	CMyROEdit();
	virtual ~CMyROEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnSetfocus();
};


