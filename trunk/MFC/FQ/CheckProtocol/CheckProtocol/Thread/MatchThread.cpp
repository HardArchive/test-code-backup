#include "StdAfx.h"
#include "MatchThread.h"
#include "PackThread.h"
#include "../Dialog/LoginTestDlg.h"

CMatchThread::CMatchThread(void)
{

}

CMatchThread::~CMatchThread(void)
{
	
}

bool CMatchThread::Initialization()
{
	return true;
}

void CMatchThread::WorkThread(LPVOID pParam)
{
	while(!m_bStop)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex,INFINITE))
		{
			if(CPackThread::m_MulMapInterceptedInfo.empty() || CGlobalData::m_listInterceptedInfo.empty())
			{
				ReleaseMutex(m_hMutex);
				continue;
			}
			else
			{
				list <INTERCEPTEDINFO>::iterator pVectIntercepted = CGlobalData::m_listInterceptedInfo.begin();
				for(pVectIntercepted; pVectIntercepted != CGlobalData::m_listInterceptedInfo.end(); ++pVectIntercepted)
				{
					if(0<(CPackThread::m_MulMapInterceptedInfo.count(pVectIntercepted->Item)))
					{
						//Xt:�ص�����֪ͨ---------- ????????????????????
// 						if(!CloseWebProcess(pVectIntercepted->Item))				            //Xt:�ر������
//  						{
// 						    //��ӡɾ������-------- ???????????????
//  						}
					    if(0>=CPackThread::m_MulMapInterceptedInfo.erase(pVectIntercepted->Item))
						{
							//��ӡɾ������-------- ???????????????
						}

						if(CLoginTestDlg::m_pThis)
						{
							CalMatchState = CLoginTestDlg::UpdataMatchState;
							CalMatchState(pVectIntercepted->Item);
						}
					}
				}
			}
		}
		ReleaseMutex(m_hMutex);
	}
}

bool CMatchThread::CloseWebProcess(char *ItemName)
{
	if(WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex,INFINITE))
	{
		map<string,DWORD>::iterator pMapWebHwnd  = CGlobalData::m_mapCloseWebInfo.find(ItemName);
		if(pMapWebHwnd != CGlobalData::m_mapCloseWebInfo.end())	
		{
			DWORD dwProcessID = pMapWebHwnd->second;
			if(dwProcessID)
			{
				HANDLE hProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS,false,dwProcessID);   //Xt:ͨ������ID��ȡ���̾��
				if(hProcessHandle)
				{
					//TerminateProcess(hProcessHandle,0);                                        //Xt:�ر�IE����
					//::SendMessage(hProcessHandle.m_hWnd,WM_CLOSE,0,0);
				}
				else
				{
					CGlobalData::PrintLogInfo("CMatchThread","ƥ��ʱ��ý��̾������!",'E',GetLastError());
					return false;
				}
			}
			else
			{
				CGlobalData::PrintLogInfo("CMatchThread","ƥ��ʱ��ý���ID����!",'E');
				return false;
			}
		}
		else
		{
			CGlobalData::PrintLogInfo("CMatchThread","ƥ��ʱ������Ŀ���ƴ���!",'E');
			return false;
		}
	}
	ReleaseMutex(m_hMutex);
	return TRUE;
}

void CMatchThread::Close()
{
	CloseThread();
}