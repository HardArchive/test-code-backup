// ListenSocket.cpp : ʵ���ļ�
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
	// TODO: �ڴ����ר�ô����/����û���
	if(nErrorCode!=0)
		return;
	
	//�����̣߳���������
	CWorkThread* pWorkThread = (CWorkThread*)AfxGetThread();
	pWorkThread->Accept(*this);

	CAsyncSocket::OnAccept(nErrorCode);
}
