

#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;
#define WM_THREADSTART    WM_USER+1
#define WM_THREADEND      WM_USER+2
#define WM_THREADADD      WM_USER+3
#define WM_THREADPOOLEND  WM_USER+4   //�̳߳ؽ���
#define WM_THREADPOOLIDIL WM_USER+5
/***************************************************************************************************
* 1�� Class      �� CThreadPool.c
* 2�� Version    �� *.*
* 3�� Description�� 
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-10-17 10:08:32
* 6�� History    �� 
* 7�� Remark     �� �������ڲ�����������vector��һ���Ǵ��浱ǰ���е��̣߳�����һ���Ǵ���ȴ����е��̣߳�
*				    �����m_nMaxThreadCount������������init�̳߳ص�ʱ����һ��PeekMessage(&msg,0,WM_USER,WM_USER,PM_NOREMOVE);��䣬
*				    ��ǿ�ƴ�����Ϣ���С�
*				    Ȼ�����Ǳ�������̵߳���Ϣ�����н�����Ϣ�������̵߳����У�
*				    ��idilʱ���̳߳ػ��Զ��Ƴ���ǰ����vector����ֹ���̣߳��ӵȴ�vector����������е��̣߳�
*				    �����WM_THREADSTART��Ϣ�����ȼ���ȷ�����߳��ܼ�ʹ����
****************************************************************************************************/
class CThreadPool
{
public:
	typedef DWORD (WINAPI *ThreadFunc)(LPVOID);
	//�߳̽ṹ
	typedef struct _sThread
	{
		HANDLE m_hThread;    //�߳̾��
		ThreadFunc m_fFunc;  //�߳�ִ�к���ָ��
		void    *m_vParam;   //�̲߳���
		DWORD  m_dRetval;    //GetExitCodeThreadȡ�õ��߳��˳�����
	} sThread, *spThread;
	typedef void (*CallBackFunc)(UINT, spThread);

	CThreadPool();
	~CThreadPool();

	void AddTask(ThreadFunc, LPVOID, bool = 0 ); //���ӵȴ������߳�
	void Init(CallBackFunc, int = 10000);   //��ʼ���̳߳�
	DWORD GetRunThreadCount();         //ȡ�õ�ǰ�����߳�����
	spThread GetRunThreadItem(int);    //�õ���ǰ�����߳̽ṹ
	DWORD GetWaitThreadCount();        //ȡ�õȴ��߳�����
	spThread GetWaitThreadItem(int);   //ȡ�õȴ������߳̽ṹ

private:
	static DWORD WINAPI Thread(LPVOID);  //�����߳�ʵ�ֺ��� 
	DWORD m_iThread;
	HANDLE m_hThread;          //�����߳̾��
	CallBackFunc m_fFunc;
	DWORD m_nMaxThreadCount;   //����߳�����
	vector<spThread> m_qRun;   //��ǰ�����߳�

	vector<spThread> m_qWait;  //�ȴ��е��߳�
	HANDLE m_hEvent;
	CRITICAL_SECTION m_cs;     //�ٽ�������
};
