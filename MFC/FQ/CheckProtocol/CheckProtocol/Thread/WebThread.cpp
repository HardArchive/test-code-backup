
#include "stdafx.h"
#include <mshtml.h>
#include "WebThread.h"
#include "../Dialog/LoginTestDlg.h"

int CWebThread::m_iWebSytpe = WEB_VISIBLE;

CWebThread::CWebThread()
{

}

CWebThread::~CWebThread()
{
     
}

BOOL CWebThread::LonginNetWork(IWebBrowser2 *pBrowser,LoginFileContentInfo *pLoginFileContentInfo)
{
	if(NULL == pBrowser)
	{
		CGlobalData::PrintLogInfo("CWebThread","IWebBrowser2 指针为空!",'E',GetLastError());
		return FALSE;
	}

	HRESULT hResult;
	IDispatch* pDisp = NULL;
	hResult = pBrowser->get_Document(&pDisp);                                        //Xt: 获取浏览器的HTML网页文档
	if(S_OK != hResult || NULL == pDisp)
	{
		if(pDisp) 
		{
			pDisp->Release();
			pDisp = NULL;
		}
		CGlobalData::PrintLogInfo("CWebThread","获取网页文档浏览器错误!",GetLastError());
		return FALSE;
	}

	IHTMLDocument2* pHTMLDocument2;
	hResult = pDisp->QueryInterface( IID_IHTMLDocument2,(void**)&pHTMLDocument2);    //Xt: 获取文档IHTMLDoument的接口
	if ( S_OK != hResult || NULL == pHTMLDocument2)
	{
		if(pHTMLDocument2)
		{
			pDisp->Release();
			pHTMLDocument2->Release();
			pDisp = NULL;
			pHTMLDocument2 = NULL;
		}
		CGlobalData::PrintLogInfo("CWebThread","获取IHTMLDocument2 接口错误!",GetLastError());
		pHTMLDocument2->Release();
		return FALSE;
	}

	IHTMLElementCollection * pIHTMLElementCollection;
	hResult = pHTMLDocument2->get_all(&pIHTMLElementCollection);                     //Xt: 获取所有HTML上控件的集合
	if(S_OK != hResult || NULL == pIHTMLElementCollection )
	{
		if(pIHTMLElementCollection)
		{
			pDisp->Release();
			pHTMLDocument2->Release();
			pIHTMLElementCollection->Release();
			pDisp = NULL;
			pHTMLDocument2 = NULL;
			pIHTMLElementCollection = NULL;
		}
		CGlobalData::PrintLogInfo("CWebThread","获取一个接口指针到一个从零开始的集合在一个HTML文档中的所有元素错误!",GetLastError());
		return FALSE;
	}

	long lLength;
	hResult = pIHTMLElementCollection->get_length(&lLength);                         //Xt: 获取HTML上控件的个数
	if(S_OK != hResult || lLength<=0)
	{
		if(pIHTMLElementCollection)
		{
			pDisp->Release();
			pHTMLDocument2->Release();
			pIHTMLElementCollection->Release();

			pDisp = NULL;
			pHTMLDocument2 = NULL;
			pIHTMLElementCollection = NULL;
		}
		CGlobalData::PrintLogInfo("CWebThread","获得HTML中的数量小等于0!",'E');
		return FALSE;
	}

	CString strWrite;
	VARIANT id, index;
	CString strHTMLName;
	for ( int i=0; i<lLength; i++ )		                                             //Xt: 循环查找HTML中控件对象
	{
		IDispatch* pDisp2;
		V_VT(&id) = VT_I4;
		V_I4(&id) = i;
		V_VT(&index) = VT_I4;
		V_I4(&index) = 0;
		hResult = pIHTMLElementCollection->item(id,index,&pDisp2 );
		if(	S_OK == hResult && NULL != pDisp2)
		{
			IHTMLElement* pElem;
			hResult = pDisp2->QueryInterface(IID_IHTMLElement,(void **)&pElem);
			if ( hResult == S_OK )
			{
				BSTR bstr;

				IHTMLInputTextElement* pUser;
				hResult = pDisp2->QueryInterface(IID_IHTMLInputTextElement,(void **)&pUser);
				if ( hResult == S_OK )
				{
					if(pLoginFileContentInfo->UserNameMark[0] != '\0')                //Xt: 登陆操作
					{
						pUser->get_name(&bstr);			                              //Xt: 获得HTML用户名控件
						strHTMLName=bstr;                                              
						if(strHTMLName == pLoginFileContentInfo->UserNameMark)        //Xt: 判断控件名称是否是登录的用户名
						{
							strWrite = pLoginFileContentInfo->UserName;
							pUser->put_value(::SysAllocString(strWrite));             //Xt：往控件添加内容
							strWrite.Empty();
						}
						else if(strHTMLName == pLoginFileContentInfo->PassWordMark)	  //Xt: 获得HTML密码控件名
						{
							strWrite = pLoginFileContentInfo->Password;
							pUser->put_value(::SysAllocString(strWrite));   
							strWrite.Empty();
							pUser->Release();
						}
					}
					if(2 == pLoginFileContentInfo->OperateItem)                       //Xt: 发帖回帖操作
					{
						pUser->get_name(&bstr);	
						strHTMLName=bstr;                                              
						if(strHTMLName == pLoginFileContentInfo->SendKey)
						{
							strWrite = pLoginFileContentInfo->SendContent;
							pUser->put_value(::SysAllocString(strWrite));
						}
					}
				}
				else
				{
					//Xt:获得HTML中Button控件指针
					IHTMLInputButtonElement* pButton;                            
					hResult = pDisp2->QueryInterface(IID_IHTMLInputButtonElement,(void **)&pButton);    
					if ( hResult == S_OK && NULL != pButton)
					{
						pButton->get_value(&bstr);                                    //Xt: 获得button值
						strHTMLName=bstr;
						if (strHTMLName==pLoginFileContentInfo->ButtonName)           //Xt: 判断是否是登录值
						{
							pElem->click();		                                      //Xt: 敲击Button键					
							i=lLength;
							WaitWebCompletionStatus(pBrowser);

							if(CLoginTestDlg::m_pThis)
							{
								CalWetStat = CLoginTestDlg::UpdataWebState;
								CalWetStat(pLoginFileContentInfo->Item);
							}
						}
						pButton->Release();
					}
				}
			}
		}
		if(pDisp2)	pDisp2->Release();
		pDisp2 = NULL;
	}

	pDisp->Release();
	pDisp = NULL;
	pHTMLDocument2->Release();
	pHTMLDocument2 = NULL;
	pIHTMLElementCollection->Release();
	pIHTMLElementCollection = NULL;

	return TRUE;
}

BOOL CWebThread::OpenBrowser(IWebBrowser2 **pBrowser,CString strURL,int iWebType)
{
	if(strURL.IsEmpty())
	{
		CGlobalData::PrintLogInfo("CWebThread","输入网址信息为空!",'E',GetLastError());
		return FALSE;
	}

	HRESULT hResult;
	hResult =  CoCreateInstance (CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (LPVOID *)pBrowser);   //得到IWebBrowser2指针
	if(S_OK != hResult)
	{
		CGlobalData::PrintLogInfo("CWebThread","初始化IWebBrowser2指针错误!",GetLastError());
		return FALSE;
	}

	if(!SetWebStatus(pBrowser,iWebType))                                                    //Xt：?????????????设置网页信息
	{
		return FALSE;
	}

	COleVariant valueURL(strURL);
	COleVariant valueNULL;
	if(S_OK != (*pBrowser)->Navigate2(valueURL,valueNULL,valueNULL,valueNULL,valueNULL))    //Xt: 打开指定网站的浏览器
	{
		CGlobalData::PrintLogInfo("CWebThread","打开浏览器错误!",GetLastError());
		return FALSE;
	}
	WaitWebCompletionStatus(*pBrowser);
	return TRUE;
}

BOOL CWebThread::SetWebStatus(IWebBrowser2 **pBrowser,int iWebType)
{
	if( WEB_VISIBLE == (iWebType & WEB_VISIBLE))
	{
		if(S_OK != (*pBrowser)->put_Visible( TRUE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览器是否可见!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_Visible( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览器是否可见!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_SILENT == (iWebType & WEB_NO_SILENT))
	{
		if(S_OK != (*pBrowser)->put_Silent( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览器关闭声音!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_Silent( TRUE ))        
		{		
			return FALSE;
		}
	}

	if( WEB_FULLSCREEN == (iWebType & WEB_FULLSCREEN))
	{
		if(S_OK != (*pBrowser)->put_FullScreen( TRUE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览器全屏!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_FullScreen( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览器全屏!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_ADDRESSBAR == (iWebType & WEB_NO_ADDRESSBAR))
	{
		if(S_OK != (*pBrowser)->put_AddressBar( FALSE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览地址栏!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_AddressBar( TRUE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览地址栏!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_MENUBAR == (iWebType & WEB_NO_MENUBAR))
	{
		if(S_OK != (*pBrowser)->put_MenuBar( FALSE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览菜单栏!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_MenuBar( TRUE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览菜单栏!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_TOOLBAR == (iWebType & WEB_NO_TOOLBAR))
	{
		if(S_OK != (*pBrowser)->put_ToolBar( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览工具栏!",'E',GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_ToolBar( TRUE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","设置浏览工具栏!",'E',GetLastError());
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CWebThread::WaitWebCompletionStatus(IWebBrowser2 *pBrowser)
{
	HRESULT hResult;
	bool bIsRun = false;
	SHANDLE_PTR  hPTR;
	while(!bIsRun)
	{
		hResult = pBrowser->get_HWND(&hPTR);	               //Xt: 实时查看浏览器是否被关闭
		if(S_OK != hResult || NULL == hPTR)
		{
			bIsRun = TRUE; 
			break;
		}

		BSTR strStatus = NULL;
		hResult = pBrowser->get_StatusText(&strStatus);        //Xt: 实时查看游览器的状态是否已经打开浏览器
		CString strStatusN = strStatus;
		if(S_OK != hResult || strStatusN == TEXT("完毕") || strStatusN == TEXT("完成" ) || strStatusN==TEXT("Done") )
		{
			bIsRun = TRUE;
			break;
		}
		Sleep(100);
	}
	return TRUE;
}

void CWebThread::WorkThread(LPVOID pParam)
{
	if(!SUCCEEDED( CoInitialize(NULL) ))  
	{
		CGlobalData::PrintLogInfo("CWebThread","初始化COM错误!",'E',GetLastError());
		return;
	}
	if(CGlobalData::m_pGlbalData)
	{
		while(!m_bStop)
		{
			IWebBrowser2 *pWebBrowser2 = NULL;

			map <string,LoginFileContentInfo>::iterator itorMapLoginFileInfo = CGlobalData::m_mapHttpFileInfo.begin();
  			for(itorMapLoginFileInfo ;itorMapLoginFileInfo != CGlobalData::m_mapHttpFileInfo.end(); ++itorMapLoginFileInfo)
 			{
				if(itorMapLoginFileInfo->second.bSelect)
				{
					//打开页面阶段
  					CString strURL(itorMapLoginFileInfo->second.URL);
					if(!OpenBrowser(&pWebBrowser2,strURL,m_iWebSytpe))
					{
						//MessageBox(m_strError);
						if(pWebBrowser2)
						{
							pWebBrowser2->Release();
							pWebBrowser2 = NULL;
						}
						continue;
					}
					//登录阶段
					if(!LonginNetWork(pWebBrowser2,&(itorMapLoginFileInfo->second)))
					{
						//AfxMessageBox();                                                                //Xt:?????????????????
						//sprintf_s(CGlobalData::m_strError,sizeof(CGlobalData::m_strError),,);
						//CGlobalData::PrintLogInfo("CWebThread","Web登录%s",itorMapLoginFileInfo->second.Item);
					}

					if(pWebBrowser2)	
					{
						if(WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex,INFINITE))
						{
							SHANDLE_PTR    spHWND;
							DWORD          dwProcessID = 0;
							DWORD          dwThreadID  = 0;
							if(S_OK == (pWebBrowser2->get_HWND(&spHWND)))
							{
								if(dwThreadID = ::GetWindowThreadProcessId(HWND(spHWND),&dwProcessID))     //Xt:得到进程ID和线程主ID
								{
									CGlobalData::m_mapCloseWebInfo.insert(map<string,DWORD>::value_type(itorMapLoginFileInfo->first,dwProcessID));
								}
							}
					    }
					    ReleaseMutex(m_hMutex);
					    pWebBrowser2->Release();
					    pWebBrowser2 = NULL;
					}
				}
			}
			break;
		}
	}
	else
	{
		CGlobalData::PrintLogInfo("CWebThread","程序没有初始化",'E');
	}
	CoUninitialize();  
}

void CWebThread::Close()
{
	CloseThread();
}