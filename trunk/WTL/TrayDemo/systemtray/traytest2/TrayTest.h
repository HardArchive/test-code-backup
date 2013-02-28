/////////////////////////////////////////////////////////////////////////////
// Project：TRAYTEST
// Author：Paul Dilascia
// Date：星期四, 十一月 07, 2002
// Description：一个简单的系统托盘程序例子，将API函数封装在一个C++类中，实现托盘程序的扩展。
//
/////////////////////////////////////////////////////////////////////////////
// TrayTest.h : main header file for the TRAYTEST application
//

#if !defined(AFX_TRAYTEST_H__E52BEFDE_E11D_4205_A949_E21BD891F4CF__INCLUDED_)
#define AFX_TRAYTEST_H__E52BEFDE_E11D_4205_A949_E21BD891F4CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyApp:
// See TrayTest.cpp for the implementation of this class
//

class CMyApp : public CWinApp
{
public:
	CMyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CMyApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYTEST_H__E52BEFDE_E11D_4205_A949_E21BD891F4CF__INCLUDED_)
