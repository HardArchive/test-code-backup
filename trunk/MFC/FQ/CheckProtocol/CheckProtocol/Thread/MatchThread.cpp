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
						//Xt:回调函数通知---------- ????????????????????
// 						if(!CloseWebProcess(pVectIntercepted->Item))				            //Xt:关闭浏览器
//  						{
// 						    //打印删除错误-------- ???????????????
//  						}
					    if(0>=CPackThread::m_MulMapInterceptedInfo.erase(pVectIntercepted->Item))
						{
							//打印删除错误-------- ???????????????
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
				HANDLE hProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS,false,dwProcessID);   //Xt:通过进程ID获取进程句柄
				if(hProcessHandle)
				{
					//TerminateProcess(hProcessHandle,0);                                        //Xt:关闭IE进程
					//::SendMessage(hProcessHandle.m_hWnd,WM_CLOSE,0,0);
				}
				else
				{
					CGlobalData::PrintLogInfo("CMatchThread","匹配时获得进程句柄错误!",'E',GetLastError());
					return false;
				}
			}
			else
			{
				CGlobalData::PrintLogInfo("CMatchThread","匹配时获得进程ID错误!",'E');
				return false;
			}
		}
		else
		{
			CGlobalData::PrintLogInfo("CMatchThread","匹配时检索项目名称错误!",'E');
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