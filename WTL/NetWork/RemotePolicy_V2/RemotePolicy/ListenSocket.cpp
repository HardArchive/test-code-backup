// ListenSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "RemotePolicy.h"
#include "ListenSocket.h"

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}

void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(nErrorCode!=0)
		return;
	
	//启动线程，接收数据
	CWorkThread* pWorkThread = (CWorkThread*)AfxGetThread();
	pWorkThread->Accept(*this);

	CAsyncSocket::OnAccept(nErrorCode);
}
