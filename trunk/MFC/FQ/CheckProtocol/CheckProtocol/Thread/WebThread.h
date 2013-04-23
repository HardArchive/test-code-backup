#pragma once
#include "ThreadBase.h"
typedef void (__stdcall *CALWEBSTATE)(char *pItemName);

#define WEB_VISIBLE             1			//网页不可见
#define WEB_NO_SILENT           2			//网页无声
#define WEB_FULLSCREEN          4           //网页全屏
#define WEB_NO_ADDRESSBAR       8           //网页地址栏不可见
#define WEB_NO_MENUBAR	       16			//网页菜单不可见
#define WEB_NO_TOOLBAR         32           //网页工具栏不可见

class CWebThread : public CThreadBase
{

public:
	CWebThread();
    ~CWebThread();

	bool LogInWeb();                                                                                //Xt:登录
	bool SendAndReplyWeb();                                                                         //Xt:发送  
	BOOL OpenBrowser(IWebBrowser2 **pBrowser,CString strURL,int iWebType);                          //Xt:打开网页
	BOOL WaitWebCompletionStatus(IWebBrowser2 *pBrowser);                                           //Xt:判断浏览器是否完全打开	
	BOOL SetWebStatus(IWebBrowser2 **pBrowser,int iWebType);                                        //Xt:设置网页功能包括、是否可见、最大化、最小化、工具栏.....
    BOOL LonginNetWork(IWebBrowser2 *pBrowser,LoginFileContentInfo *pLoginFileContentInfo);	    	//Xt:自动登录
	void Close();

	void SetWebStatusInfo(int iWebType = WEB_VISIBLE)	 { m_iWebSytpe = iWebType; }                //Xt:获得网页功能
	virtual void WorkThread(LPVOID pParam = NULL);
	virtual bool Initialization() { return true; }

	CALWEBSTATE CalWetStat;
public: 
	static  int	m_iWebSytpe; 
};


