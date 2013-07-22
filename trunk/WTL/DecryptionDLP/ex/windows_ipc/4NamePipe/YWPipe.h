/*
 *	�����ܵ�ͨ����,Ŀǰ֧�ֵ������������ͻ������ӣ�֧��˫��ͨ�š�
 *	2006��1��5�� ������
 */
#pragma once

#include <string>
#include <windows.h>
#include <stdio.h>
using namespace std;
#define BUFSIZE		4096

#define YW_EVENT_SER_CRE_OK        	0		/*! Create Server OK */
#define YW_EVENT_SER_CRE_FAIL		1		/*! Create Server FALSE */
#define YW_EVENT_CLI_CONNECT		2		/*! an client has connect */
#define YW_EVENT_CLI_DROP			3		/*! an client has left 	*/

#define YW_EVENT_CLI_CRE_OK        	4		/*! Create Client OK */
#define YW_EVENT_CLI_CRE_FAIL		5		/*! Create Client FALSE */
#define YW_EVENT_SER_CONNECT		6		/*! Connect to Server*/
#define YW_EVENT_SER_DROP			7		/*! Disconnect from Server 	*/

class CYWPipe
{
public:
	CYWPipe(void);
	virtual ~CYWPipe(void);
public:
	string GetError() {	return ErrorMessage;}
	string GetSerPipeName() {return SerPipeName;}
	string GetCliPipeName() {return CliPipeName;}
private:
	bool IsService;//true Ϊ�������ˣ�false Ϊ�ͻ���

	HANDLE hSerPipe;//�������ܵ����
	bool m_bSerThreadAlive;//�������̱߳�־
	string SerPipeName;//�ܵ�����
	bool CreateSerPipe();//����������
	static DWORD ThrSer(LPVOID pParam);//�������߳�
	
	HANDLE hCliPipe;//�ͻ��˾��
	bool m_bCliThreadAlive;//�ͻ����߳̾��
	string CliPipeName;//�ܵ�����
	bool CreateCliPipe();//�����ͻ���
	static DWORD ThrCli(LPVOID pParam);//�ͻ����߳̾��
public:
	BOOL SendData(void * lpBuf, int nBufLen);//�ӹܵ���������
	BOOL Create(LPSTR PipeName,bool IsSer);//PipeName�ܵ���,IsSer=trueΪ������,falseΪ�ͻ���
	BOOL Close();//�رչܵ�����

	virtual int OnRecData(LPVOID vlpPara, int len);//���������¼���Ӧ
	virtual int OnEvent(int EventType);//���ӶϿ��¼���Ӧ

private:
	HANDLE hSerWaitConEvent;//�������ȴ������¼�
	HANDLE hSerRDEvent;//��������ȡ�����¼�
	HANDLE hCliWaitConEvent;//�ͻ��˵ȴ������¼�
	HANDLE hCliRDEvent;//�ͻ��˶�ȡ�����¼�
	string ErrorMessage;//������Ϣ
};
