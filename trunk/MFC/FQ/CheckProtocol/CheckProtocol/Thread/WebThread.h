#pragma once
#include "ThreadBase.h"
typedef void (__stdcall *CALWEBSTATE)(char *pItemName);

#define WEB_VISIBLE             1			//��ҳ���ɼ�
#define WEB_NO_SILENT           2			//��ҳ����
#define WEB_FULLSCREEN          4           //��ҳȫ��
#define WEB_NO_ADDRESSBAR       8           //��ҳ��ַ�����ɼ�
#define WEB_NO_MENUBAR	       16			//��ҳ�˵����ɼ�
#define WEB_NO_TOOLBAR         32           //��ҳ���������ɼ�

class CWebThread : public CThreadBase
{

public:
	CWebThread();
    ~CWebThread();

	bool LogInWeb();                                                                                //Xt:��¼
	bool SendAndReplyWeb();                                                                         //Xt:����  
	BOOL OpenBrowser(IWebBrowser2 **pBrowser,CString strURL,int iWebType);                          //Xt:����ҳ
	BOOL WaitWebCompletionStatus(IWebBrowser2 *pBrowser);                                           //Xt:�ж�������Ƿ���ȫ��	
	BOOL SetWebStatus(IWebBrowser2 **pBrowser,int iWebType);                                        //Xt:������ҳ���ܰ������Ƿ�ɼ�����󻯡���С����������.....
    BOOL LonginNetWork(IWebBrowser2 *pBrowser,LoginFileContentInfo *pLoginFileContentInfo);	    	//Xt:�Զ���¼
	void Close();

	void SetWebStatusInfo(int iWebType = WEB_VISIBLE)	 { m_iWebSytpe = iWebType; }                //Xt:�����ҳ����
	virtual void WorkThread(LPVOID pParam = NULL);
	virtual bool Initialization() { return true; }

	CALWEBSTATE CalWetStat;
public: 
	static  int	m_iWebSytpe; 
};


