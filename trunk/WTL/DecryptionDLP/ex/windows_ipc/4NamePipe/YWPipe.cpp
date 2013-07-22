#include "StdAfx.h"
#include ".\ywpipe.h"

CYWPipe::CYWPipe(void)
{
	ErrorMessage="";
	SerPipeName="";
	CliPipeName="";
	hSerPipe=NULL;
	hCliPipe=NULL;
	hSerWaitConEvent=NULL;
	hCliWaitConEvent=NULL;
	m_bSerThreadAlive=false;
	m_bCliThreadAlive=false;
	hSerRDEvent=NULL;
	hCliRDEvent=NULL;
}

CYWPipe::~CYWPipe(void)
{
	Close();
}

BOOL CYWPipe::Close()
{
	m_bSerThreadAlive=false;
	m_bCliThreadAlive=false;
	SetEvent(hSerWaitConEvent);//�����ڵȴ��ͻ������ӣ�ʹ֮����
	SetEvent(hCliWaitConEvent);//�����ڵȴ���������ӣ�ʹ֮����
	SetEvent(hSerRDEvent);//�����ڵȴ�����˶�ȡ���ݣ�ʹ֮����
	SetEvent(hCliRDEvent);//�����ڵȴ��ͻ��˶�ȡ���ݣ�ʹ֮����
	Sleep(20);

	if(hSerPipe!=NULL){CloseHandle(hSerPipe);hSerPipe=NULL;}
	if(hCliPipe!=NULL){	CloseHandle(hCliPipe);hCliPipe=NULL;}
	if(hSerWaitConEvent!=NULL) { CloseHandle(hSerWaitConEvent);hSerWaitConEvent=NULL;}
	if(hSerRDEvent!=NULL){CloseHandle(hSerRDEvent);hSerRDEvent=NULL;}
	if(hCliWaitConEvent!=NULL) { CloseHandle(hCliWaitConEvent);hCliWaitConEvent=NULL;}
	if(hCliRDEvent!=NULL){CloseHandle(hCliRDEvent);hCliRDEvent=NULL;}

	return 0;
}
BOOL CYWPipe::Create(LPSTR PipeName,bool IsSer)//PipeName�ܵ���,IsSer=trueΪ������
{
	char NameBuf[100];
	sprintf(NameBuf, "\\\\.\\pipe\\%s",PipeName);
	TRACE("PipeName: %s\n",NameBuf);
	if(m_bSerThreadAlive && m_bCliThreadAlive) {ErrorMessage="Create Failed,Thread is alive";return FALSE;}
	if(IsSer)
	{
		SerPipeName=NameBuf;
		if(CreateSerPipe()) return TRUE;
		ErrorMessage="Create Server Pipe Failed";
		return FALSE;
	}
	else
	{
		CliPipeName=NameBuf;
		if(CreateCliPipe()) return TRUE;
		ErrorMessage="Create Client Pipe Failed";
		return FALSE;
	}
}
bool CYWPipe::CreateSerPipe()
{
	if(hSerWaitConEvent!=NULL){CloseHandle(hSerWaitConEvent);hSerWaitConEvent=NULL;}
	hSerWaitConEvent=CreateEvent(NULL,FALSE,FALSE,NULL);//��ʼ���ȴ������¼�
	if(hSerWaitConEvent==NULL) return false;

	if(hSerRDEvent!=NULL){CloseHandle(hSerRDEvent);hSerRDEvent=NULL;}
	hSerRDEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	if(hSerRDEvent==NULL) return false;
	
	HANDLE handle;
	handle=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrSer,(LPVOID)this,0,NULL);
	if(handle==NULL) return false;
	IsService=true;
	return true;
}
bool CYWPipe::CreateCliPipe()
{
	if(hCliWaitConEvent!=NULL){CloseHandle(hCliWaitConEvent);hCliWaitConEvent=NULL;}
	hCliWaitConEvent=CreateEvent(NULL,FALSE,FALSE,NULL);//��ʼ���ȴ������¼�
	if(hCliWaitConEvent==NULL) return false;

	if(hCliRDEvent!=NULL){CloseHandle(hCliRDEvent);hCliRDEvent=NULL;}
	hCliRDEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	if(hCliRDEvent==NULL) return false;

	HANDLE handle;
	handle=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThrCli,(LPVOID)this,0,NULL);
	if(handle==NULL) return false;
	IsService=false;
	return true;
}
DWORD CYWPipe::ThrSer(LPVOID pParam)
{
	CYWPipe *pMyPipe=(CYWPipe *)pParam;

	pMyPipe->m_bSerThreadAlive=true;
	while(1)
	{
		if(!pMyPipe->m_bSerThreadAlive) break;//�˳��߳�
		pMyPipe->hSerPipe=CreateNamedPipe(pMyPipe->SerPipeName.c_str (),
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,//�����ص�ģʽִ�����ӡ�����д
			0,1,BUFSIZE,BUFSIZE,1000,NULL);
		if(INVALID_HANDLE_VALUE==pMyPipe->hSerPipe)
		{
			pMyPipe->ErrorMessage="Server Thread::CreateNamedPipe Function Failed";
			pMyPipe->hSerPipe=NULL;
			pMyPipe->OnEvent(YW_EVENT_SER_CRE_FAIL);//������������ʧ��
			break;
		}
		TRACE("Service pipe has been created!\n");
		
		OVERLAPPED ovlap;
		DWORD timeout=100;
		DWORD ret;
	
		ZeroMemory(&ovlap,sizeof(OVERLAPPED));
		ovlap.hEvent=pMyPipe->hSerWaitConEvent;
		if(!ConnectNamedPipe(pMyPipe->hSerPipe,&ovlap))//�ȴ��ͻ�����
		{
			if(ERROR_IO_PENDING!=GetLastError())
			{
				pMyPipe->ErrorMessage="Server Thread::ConnectNamedPipe Function Failed";
				CloseHandle(pMyPipe->hSerPipe);
				pMyPipe->hSerPipe=NULL;
				pMyPipe->OnEvent(YW_EVENT_SER_CRE_FAIL);//������������ʧ��
				//TRACE("***%d***\n",pMyPipe->hSerPipe);
				continue;
			}
		}
		pMyPipe->OnEvent(YW_EVENT_SER_CRE_OK);//�����������ɹ�
		
		TRACE("Wait Client Connect......   ");
		while(1)
		{
			ret=WaitForSingleObject(pMyPipe->hSerWaitConEvent,timeout);
			if(ret==WAIT_FAILED)
			{
				pMyPipe->ErrorMessage="Server Thread::WaitForSingleObject Function Failed";
				TRACE("Wait Client Connect Failed!\n");
				CloseHandle(pMyPipe->hSerPipe);
				pMyPipe->hSerPipe=NULL;
				return 0;
			}
			if(ret==WAIT_OBJECT_0)//�ͻ������ӳɹ�
			{
				if(!pMyPipe->m_bSerThreadAlive)//Ҫ���˳��߳�
				{
					CloseHandle(pMyPipe->hSerPipe);
					pMyPipe->hSerPipe=NULL;
					return 0;
				}
				TRACE("Client Connect connect complete!\n");
				break;
			}
			if(ret==WAIT_TIMEOUT)//�ȴ����ӳ�ʱ
			{
				if(!pMyPipe->m_bSerThreadAlive)//Ҫ���˳��߳�
				{
					CloseHandle(pMyPipe->hSerPipe);
					pMyPipe->hSerPipe=NULL;
					return 0;
				}
			}
		}
		pMyPipe->OnEvent(YW_EVENT_CLI_CONNECT);//�ͻ�������
		if(!pMyPipe->m_bSerThreadAlive) return 0;//Ҫ���˳��߳�
		//�ɹ����ӽ�����һ������
		TRACE("[%d]Service pipe Run!\n",pMyPipe->hSerPipe);
		
		char Buffer[BUFSIZE];

		//HANDLE hSerEventR;
		//hSerEventR=CreateEvent(NULL,FALSE,FALSE,NULL);

		OVERLAPPED gOverlapped;
		// set up overlapped structure fields
		ZeroMemory(&gOverlapped,sizeof(OVERLAPPED));
		gOverlapped.hEvent     = pMyPipe->hSerRDEvent; 

		timeout=1000;
		ret=0;
		
		BOOL bResult=FALSE,fSuccess=FALSE;
		DWORD nBytesRead=0;
		bool DiscCliFlag=false;
		while(1)
		{
			// attempt an asynchronous read operation
			TRACE("Service::Wait Read......   ");
			bResult = ReadFile(pMyPipe->hSerPipe, Buffer, BUFSIZE, &nBytesRead, &gOverlapped) ; 
			while(1)
			{
				ret=WaitForSingleObject(pMyPipe->hSerRDEvent,timeout);
			
				if(ret==WAIT_FAILED)
				{
					pMyPipe->ErrorMessage="Server Thread::WaitForSingleObject Function Failed";
					TRACE("Read Data Error!\n");
					pMyPipe->m_bSerThreadAlive=false;
					break;
				}
				if(ret==WAIT_OBJECT_0)
				{
					//OVERLAPPED
					DWORD cbrett=0;
					fSuccess = GetOverlappedResult( 
						pMyPipe->hSerPipe,     // pipe handle 
						&gOverlapped, // OVERLAPPED structure 
						&cbrett,    // bytes transferred 
						FALSE);    // does not wait 
					if (!fSuccess) 
					{
						TRACE("********Client disconnect!********\n"); 
						CloseHandle(pMyPipe->hSerPipe);
						pMyPipe->hSerPipe=NULL;
						pMyPipe->OnEvent(YW_EVENT_CLI_DROP);//�ͻ��˶Ͽ�
						DiscCliFlag=true;
						break;
					}
					if(cbrett==BUFSIZE)
					{
						//static int bbb=0;
						//TRACE("%s (%d)\n",Buffer,bbb++);
						pMyPipe->OnRecData(Buffer,BUFSIZE);
						ZeroMemory(Buffer,BUFSIZE);
					}
					else TRACE("Not data!\n");
					break;
				}
				if(ret==WAIT_TIMEOUT)//�ȴ����ӳ�ʱ
				{
					if(!pMyPipe->m_bSerThreadAlive)//Ҫ���˳��߳�
					{
						pMyPipe->ErrorMessage="Wait Client Connect Failed";
						CloseHandle(pMyPipe->hSerPipe);
						CloseHandle(pMyPipe->hSerRDEvent);
						pMyPipe->hSerPipe=NULL;
						pMyPipe->hSerRDEvent=NULL;
						return 0;
					}
					static int bbb=0;
					//TRACE("Service:: [%d] Read data Time out!%d\n",pMyPipe->hSerPipe,bbb++);
				}
				if(!pMyPipe->m_bSerThreadAlive) return 0;//Ҫ���˳��߳�
			}

			if(DiscCliFlag) break;//�Ͽ�����,Ҫ����µȴ�����
			if(!pMyPipe->m_bSerThreadAlive) return 0;//Ҫ���˳��߳�
		}
	}
	
	pMyPipe->ErrorMessage="Server Thread End";
	TRACE("end run!\n");
	return 0;
}

DWORD CYWPipe::ThrCli(LPVOID pParam)
{
	CYWPipe *pMyPipe=(CYWPipe *)pParam;

	pMyPipe->m_bCliThreadAlive=true;

	DWORD timeout=1000;
	DWORD ret=0;

	BOOL bResult=FALSE,fSuccess=FALSE;
	DWORD nBytesRead=0;

	while(1)
	{
		if(!pMyPipe->m_bCliThreadAlive) return 0;//�˳��߳�
		while(1)
		{
			if(!WaitNamedPipe(pMyPipe->CliPipeName.c_str(),NMPWAIT_USE_DEFAULT_WAIT))
			{
				if(!pMyPipe->m_bCliThreadAlive) return 0;
			}
			else break;
			//TRACE("Con't find Pipe! delay 1s \n");
			if(!pMyPipe->m_bCliThreadAlive) return 0;//�˳��߳�
			Sleep(10);//10ms������
			if(!pMyPipe->m_bCliThreadAlive) return 0;//�˳��߳�
		}
		TRACE("Existing Pipe!\n");
		if(!pMyPipe->m_bCliThreadAlive) return 0;//�˳��߳�
		OVERLAPPED Olapped;
		//HANDLE hCliEvent;
		//hCliEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
		// set up overlapped structure fields
		ZeroMemory(&Olapped,sizeof(OVERLAPPED));
		Olapped.hEvent     = pMyPipe->hCliWaitConEvent; 
		while (1) 
		{ 
			pMyPipe->hCliPipe =CreateFile(pMyPipe->CliPipeName.c_str(),GENERIC_READ | GENERIC_WRITE,
				0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);

			// Break if the pipe handle is valid. 

			if (pMyPipe->hCliPipe != INVALID_HANDLE_VALUE) 
				break; 
			WaitForSingleObject(pMyPipe->hCliWaitConEvent,0);
			// Exit if an error other than ERROR_PIPE_BUSY occurs. 

			if (GetLastError() != ERROR_PIPE_BUSY) 
			{
				TRACE("Client Could not open pipe"); 
				pMyPipe->hCliPipe=NULL;
				return 0;
			}
			if(!pMyPipe->m_bCliThreadAlive) return 0;//�˳��߳�
		}
		
		pMyPipe->OnEvent(YW_EVENT_SER_CONNECT);//�ͻ�������
		TRACE("Connect Service Successed!\n");
		if(!pMyPipe->m_bCliThreadAlive) return 0;//�˳��߳�

		//�ɹ����ӽ�����һ������
		char Buffer[BUFSIZE];

		//HANDLE hCliEventR;
		//hCliEventR=CreateEvent(NULL,FALSE,FALSE,NULL);

		OVERLAPPED gOverlapped;
		// set up overlapped structure fields
		ZeroMemory(&gOverlapped,sizeof(OVERLAPPED));
		gOverlapped.hEvent     = pMyPipe->hCliRDEvent; 

		bool DiscSerFlag=false;
		while(1)
		{
			// attempt an asynchronous read operation
			TRACE("Client::Wait Read......   ");
			//pMyPipe->SendData (NULL,0);
			bResult = ReadFile(pMyPipe->hCliPipe, Buffer, BUFSIZE, &nBytesRead, &gOverlapped) ; 
			while(1)
			{
				//TRACE("bResult=%d,Wait Signal",bResult);
				ret=WaitForSingleObject(pMyPipe->hCliRDEvent,timeout);

				if(ret==WAIT_FAILED)
				{
					TRACE("Read Data Error!\n");
					DiscSerFlag=true;
					break;
				}
				if(ret==WAIT_OBJECT_0)
				{
					//OVERLAPPED
					DWORD cbrett=0;
					fSuccess = GetOverlappedResult( 
						pMyPipe->hSerPipe,     // pipe handle 
						&gOverlapped, // OVERLAPPED structure 
						&cbrett,    // bytes transferred 
						FALSE);    // does not wait 
					if (!fSuccess) 
					{
						TRACE("********Service disconnect!********\n"); 
						pMyPipe->OnEvent(YW_EVENT_SER_DROP);//�ͻ�������
						DiscSerFlag=true;
						break;
					}
					if(cbrett==BUFSIZE)
					{
						//static ccc=0;
						//TRACE("%s (%d)\n",Buffer,ccc++);
						pMyPipe->OnRecData(Buffer,BUFSIZE);
						ZeroMemory(Buffer,BUFSIZE);
					}
					else TRACE("Not data!\n");
					break;
				}
				if(ret==WAIT_TIMEOUT)//�ȴ����ӳ�ʱ
				{
					if(!pMyPipe->m_bCliThreadAlive)//Ҫ���˳��߳�
					{
						CloseHandle(pMyPipe->hCliPipe);
						CloseHandle(pMyPipe->hCliRDEvent);
						pMyPipe->hCliPipe=NULL;
						pMyPipe->hCliRDEvent=NULL;
						return 0;
					}
					//static vvv=0;
					//TRACE("Client:: [%d] Read data Time out!%d\n",pMyPipe->hCliPipe,vvv++);
				}
				if(!pMyPipe->m_bCliThreadAlive) {TRACE("CliThr end run!\n");break;}
			}

			if(!pMyPipe->m_bCliThreadAlive) {TRACE("CliThr end run!\n");break;}
			if(DiscSerFlag) break;
		}
		CloseHandle(pMyPipe->hCliPipe);
		pMyPipe->hCliPipe=NULL;
	}

	TRACE("CliThr end run!\n");

	return 0;
}

BOOL CYWPipe::SendData(void * lpBuf, int nBufLen)
{
	DWORD dwWrite=0;
	HANDLE hEvent;
	hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	OVERLAPPED gOverlapped;
	ZeroMemory(&gOverlapped,sizeof(OVERLAPPED));
	gOverlapped.hEvent     = hEvent; 

	BOOL  bResult=0,ret=0,fSuccess=FALSE;
	DWORD timeout=1000;
	if(IsService && hSerPipe!=NULL)//����������˹ܵ�����
	{
		//TRACE("Service::Send data to Client Start!\n");
		bResult=WriteFile(hSerPipe,lpBuf,nBufLen,&dwWrite,&gOverlapped);
		if(bResult)
		{
			while(1)
			{
				ret=WaitForSingleObject(hEvent,0);

				if(ret==WAIT_FAILED)
				{
					TRACE("Service::Send data error!\n");
					break;
				}
				if(ret==WAIT_OBJECT_0)
				{
					DWORD cbrett=0;
					fSuccess = GetOverlappedResult( 
						hSerPipe,     // pipe handle 
						&gOverlapped, // OVERLAPPED structure 
						&cbrett,    // bytes transferred 
						FALSE);    // does not wait 
					if (!fSuccess) 
					{
						TRACE("Service::Client disconnect!\n"); 
						return 0;
					}
					if(cbrett==nBufLen)
					{
						//TRACE("Service::Send Data Success!\n");
					}
					else TRACE("Service::Send Data Failed!\n");
					break;
				}
				if(ret==WAIT_TIMEOUT)//�ȴ����ӳ�ʱ
				{
					//TRACE("Service::[%d] Send data Time out!\n",hSerPipe);
				}
			}
		}
	}
	if(!IsService && hCliPipe!=NULL)//�ͻ��˹ܵ�����
	{
		//TRACE("Client::Send data to Service Start!\n");
		
		bResult=WriteFile(hCliPipe,lpBuf,nBufLen,&dwWrite,&gOverlapped);
		if(bResult)
		{
			while(1)
			{
				ret=WaitForSingleObject(hEvent,0);

				if(ret==WAIT_FAILED)
				{
					TRACE("CliSend::error!\n");
					break;
				}
				if(ret==WAIT_OBJECT_0)
				{
					DWORD cbrett=0;
					fSuccess = GetOverlappedResult( 
						hCliPipe,     // pipe handle 
						&gOverlapped, // OVERLAPPED structure 
						&cbrett,    // bytes transferred 
						FALSE);    // does not wait 
					if (!fSuccess) 
					{
						TRACE("Client::Service disconnect!\n"); 
						return 0;
					}
					if(cbrett==nBufLen)
					{
						//TRACE("Client::Send Data Success!\n");
					}
					else TRACE("Client::Send Data Failed!\n");
					break;
				}
				if(ret==WAIT_TIMEOUT)//�ȴ����ӳ�ʱ
				{
					//TRACE("Client::[%d] Send data Time out!\n",hCliPipe);
				}
			}
		}
	}
	return TRUE;
}

int CYWPipe::OnRecData(LPVOID vlpPara, int len)
{
	static ccc=0;
	TRACE("%s (%d)\n",vlpPara,ccc++);
	return 0;
}
int CYWPipe::OnEvent(int EventType)
{
	switch(EventType)
	{
	case YW_EVENT_SER_CRE_OK:/*! Create Server OK */
		TRACE("*****Create Server OK*****\n");
		break;
	case YW_EVENT_SER_CRE_FAIL:/*! Create Server FALSE */
		TRACE("*****Create Server FALSE*****\n");
		break;
	case YW_EVENT_CLI_CONNECT:/*! an client has connect */
		TRACE("*****an client has connect*****\n");
		break;
	case YW_EVENT_CLI_DROP:/*! an client has left 	*/
		TRACE("*****an client has left*****\n");
		break;
	case YW_EVENT_CLI_CRE_OK:/*! Create Client OK */
		TRACE("*****Create Client OK*****\n");
		break;
	case YW_EVENT_CLI_CRE_FAIL:/*! Create Client FALSE */
		TRACE("*****Create Client FALSE*****\n");
		break;
	case YW_EVENT_SER_CONNECT:/*! Connect to Server*/
		TRACE("*****Connect to Server*****\n");
		break;
	case YW_EVENT_SER_DROP:/*! Disconnect from Server 	*/
		TRACE("*****Disconnect from Server*****\n");
		break;
	default:
		TRACE("Don't know Event[%d]!\n",EventType);
		break;
	}
	return 0;
}