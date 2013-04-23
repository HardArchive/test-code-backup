
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
		CGlobalData::PrintLogInfo("CWebThread","IWebBrowser2 ָ��Ϊ��!",'E',GetLastError());
		return FALSE;
	}

	HRESULT hResult;
	IDispatch* pDisp = NULL;
	hResult = pBrowser->get_Document(&pDisp);                                        //Xt: ��ȡ�������HTML��ҳ�ĵ�
	if(S_OK != hResult || NULL == pDisp)
	{
		if(pDisp) 
		{
			pDisp->Release();
			pDisp = NULL;
		}
		CGlobalData::PrintLogInfo("CWebThread","��ȡ��ҳ�ĵ����������!",GetLastError());
		return FALSE;
	}

	IHTMLDocument2* pHTMLDocument2;
	hResult = pDisp->QueryInterface( IID_IHTMLDocument2,(void**)&pHTMLDocument2);    //Xt: ��ȡ�ĵ�IHTMLDoument�Ľӿ�
	if ( S_OK != hResult || NULL == pHTMLDocument2)
	{
		if(pHTMLDocument2)
		{
			pDisp->Release();
			pHTMLDocument2->Release();
			pDisp = NULL;
			pHTMLDocument2 = NULL;
		}
		CGlobalData::PrintLogInfo("CWebThread","��ȡIHTMLDocument2 �ӿڴ���!",GetLastError());
		pHTMLDocument2->Release();
		return FALSE;
	}

	IHTMLElementCollection * pIHTMLElementCollection;
	hResult = pHTMLDocument2->get_all(&pIHTMLElementCollection);                     //Xt: ��ȡ����HTML�Ͽؼ��ļ���
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
		CGlobalData::PrintLogInfo("CWebThread","��ȡһ���ӿ�ָ�뵽һ�����㿪ʼ�ļ�����һ��HTML�ĵ��е�����Ԫ�ش���!",GetLastError());
		return FALSE;
	}

	long lLength;
	hResult = pIHTMLElementCollection->get_length(&lLength);                         //Xt: ��ȡHTML�Ͽؼ��ĸ���
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
		CGlobalData::PrintLogInfo("CWebThread","���HTML�е�����С����0!",'E');
		return FALSE;
	}

	CString strWrite;
	VARIANT id, index;
	CString strHTMLName;
	for ( int i=0; i<lLength; i++ )		                                             //Xt: ѭ������HTML�пؼ�����
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
					if(pLoginFileContentInfo->UserNameMark[0] != '\0')                //Xt: ��½����
					{
						pUser->get_name(&bstr);			                              //Xt: ���HTML�û����ؼ�
						strHTMLName=bstr;                                              
						if(strHTMLName == pLoginFileContentInfo->UserNameMark)        //Xt: �жϿؼ������Ƿ��ǵ�¼���û���
						{
							strWrite = pLoginFileContentInfo->UserName;
							pUser->put_value(::SysAllocString(strWrite));             //Xt�����ؼ��������
							strWrite.Empty();
						}
						else if(strHTMLName == pLoginFileContentInfo->PassWordMark)	  //Xt: ���HTML����ؼ���
						{
							strWrite = pLoginFileContentInfo->Password;
							pUser->put_value(::SysAllocString(strWrite));   
							strWrite.Empty();
							pUser->Release();
						}
					}
					if(2 == pLoginFileContentInfo->OperateItem)                       //Xt: ������������
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
					//Xt:���HTML��Button�ؼ�ָ��
					IHTMLInputButtonElement* pButton;                            
					hResult = pDisp2->QueryInterface(IID_IHTMLInputButtonElement,(void **)&pButton);    
					if ( hResult == S_OK && NULL != pButton)
					{
						pButton->get_value(&bstr);                                    //Xt: ���buttonֵ
						strHTMLName=bstr;
						if (strHTMLName==pLoginFileContentInfo->ButtonName)           //Xt: �ж��Ƿ��ǵ�¼ֵ
						{
							pElem->click();		                                      //Xt: �û�Button��					
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
		CGlobalData::PrintLogInfo("CWebThread","������ַ��ϢΪ��!",'E',GetLastError());
		return FALSE;
	}

	HRESULT hResult;
	hResult =  CoCreateInstance (CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (LPVOID *)pBrowser);   //�õ�IWebBrowser2ָ��
	if(S_OK != hResult)
	{
		CGlobalData::PrintLogInfo("CWebThread","��ʼ��IWebBrowser2ָ�����!",GetLastError());
		return FALSE;
	}

	if(!SetWebStatus(pBrowser,iWebType))                                                    //Xt��?????????????������ҳ��Ϣ
	{
		return FALSE;
	}

	COleVariant valueURL(strURL);
	COleVariant valueNULL;
	if(S_OK != (*pBrowser)->Navigate2(valueURL,valueNULL,valueNULL,valueNULL,valueNULL))    //Xt: ��ָ����վ�������
	{
		CGlobalData::PrintLogInfo("CWebThread","�����������!",GetLastError());
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
			CGlobalData::PrintLogInfo("CWebThread","����������Ƿ�ɼ�!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_Visible( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","����������Ƿ�ɼ�!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_SILENT == (iWebType & WEB_NO_SILENT))
	{
		if(S_OK != (*pBrowser)->put_Silent( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","����������ر�����!",GetLastError());
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
			CGlobalData::PrintLogInfo("CWebThread","���������ȫ��!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_FullScreen( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","���������ȫ��!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_ADDRESSBAR == (iWebType & WEB_NO_ADDRESSBAR))
	{
		if(S_OK != (*pBrowser)->put_AddressBar( FALSE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","���������ַ��!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_AddressBar( TRUE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","���������ַ��!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_MENUBAR == (iWebType & WEB_NO_MENUBAR))
	{
		if(S_OK != (*pBrowser)->put_MenuBar( FALSE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","��������˵���!",GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_MenuBar( TRUE))        
		{
			CGlobalData::PrintLogInfo("CWebThread","��������˵���!",GetLastError());
			return FALSE;
		}
	}

	if( WEB_NO_TOOLBAR == (iWebType & WEB_NO_TOOLBAR))
	{
		if(S_OK != (*pBrowser)->put_ToolBar( FALSE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","�������������!",'E',GetLastError());
			return FALSE;
		}
	}
	else
	{
		if(S_OK != (*pBrowser)->put_ToolBar( TRUE ))        
		{
			CGlobalData::PrintLogInfo("CWebThread","�������������!",'E',GetLastError());
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
		hResult = pBrowser->get_HWND(&hPTR);	               //Xt: ʵʱ�鿴������Ƿ񱻹ر�
		if(S_OK != hResult || NULL == hPTR)
		{
			bIsRun = TRUE; 
			break;
		}

		BSTR strStatus = NULL;
		hResult = pBrowser->get_StatusText(&strStatus);        //Xt: ʵʱ�鿴��������״̬�Ƿ��Ѿ��������
		CString strStatusN = strStatus;
		if(S_OK != hResult || strStatusN == TEXT("���") || strStatusN == TEXT("���" ) || strStatusN==TEXT("Done") )
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
		CGlobalData::PrintLogInfo("CWebThread","��ʼ��COM����!",'E',GetLastError());
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
					//��ҳ��׶�
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
					//��¼�׶�
					if(!LonginNetWork(pWebBrowser2,&(itorMapLoginFileInfo->second)))
					{
						//AfxMessageBox();                                                                //Xt:?????????????????
						//sprintf_s(CGlobalData::m_strError,sizeof(CGlobalData::m_strError),,);
						//CGlobalData::PrintLogInfo("CWebThread","Web��¼%s",itorMapLoginFileInfo->second.Item);
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
								if(dwThreadID = ::GetWindowThreadProcessId(HWND(spHWND),&dwProcessID))     //Xt:�õ�����ID���߳���ID
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
		CGlobalData::PrintLogInfo("CWebThread","����û�г�ʼ��",'E');
	}
	CoUninitialize();  
}

void CWebThread::Close()
{
	CloseThread();
}