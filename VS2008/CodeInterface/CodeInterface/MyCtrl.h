#pragma once
#include "afxwin.h"
#include "ClickReceiver.h"
class CMyCtrl :
	public CDialog
{
public:
	CMyCtrl(void);
	//CMyCtrl(CDialog* pDialog)
	//{

	//}
	~CMyCtrl(void);

public: 
	virtual void OnClick(); 

public: 
	ClickReceiver *m_receiver; 
public:
	////////DECLARE_MESSAGE_MAP()
	//{{AFX_MSG(CMyCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
