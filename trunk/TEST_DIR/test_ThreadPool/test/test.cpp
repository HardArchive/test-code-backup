// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include "ThreadPool.h"


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
        DWORD  m_dRetval;
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

CThreadPool::CThreadPool()
{
    m_iThread = 0;
    m_fFunc = 0;
    m_hThread = 0;
    InitializeCriticalSection(&m_cs); //��ʼ��һ���ٽ�������
}

CThreadPool::~CThreadPool()
{
	//��һ����Ϣ���루���ͣ���ָ���̵߳���Ϣ��������ȴ��̴߳�����Ϣ�ͷ���
    PostThreadMessage(m_iThread, WM_THREADPOOLEND, 0, 0); //�̳߳��˳���Ϣ
    WaitForSingleObject(m_hThread, -1); //�ȴ�һ���ں˶����Ϊ��֪ͨ״̬ INFINITE
	//Windows�ṩ�˼����ں˶�����Դ�����֪ͨ״̬��δ֪ͨ״̬�����̡��̡߳���ҵ���ļ�������̨����/���/���������¼����ȴ���ʱ�����ź������������
    CloseHandle(m_hThread);
    LeaveCriticalSection(&m_cs); //���� ��EnterCriticalSection֮�������Դ�Ѿ��ͷ��ˣ������߳̿��Խ��в�
    int i;
    for(i = 0; i < m_qRun.size(); i++)
    {
        delete m_qRun[i];
    }
    for(i = 0; i < m_qWait.size(); i++)
    {
        delete m_qWait[i];
    }
}

//ThreadFunc func,�߳�ʵ�ֺ��� LPVOID lp,�̲߳��� bool bImme //?????�Ƿ�Ϊ��һ��ִ��
void CThreadPool::AddTask(ThreadFunc func, LPVOID lp, bool bImme)
{
    if(!bImme)
    {
        PostThreadMessage(m_iThread, WM_THREADADD, (WPARAM)func, (LPARAM)lp);  //�����߳�---���߳̽ṹ����ȴ����е��߳�vector m_qWait��
    }
    else
    {
        spThread sTh = new sThread;
        sTh->m_dRetval = 0;
        sTh->m_hThread = 0;
        sTh->m_fFunc = func;
        sTh->m_vParam = lp;
        EnterCriticalSection(&m_cs);  //���� �������Ĵ��봦������в����������߳̽��в�������������LeaveCriticalSection
        m_qWait.insert(m_qWait.begin(), sTh);
        LeaveCriticalSection(&m_cs);
    }
    PostThreadMessage(m_iThread, WM_THREADPOOLIDIL, 0, 0); //?

}

//��ʼ���̳߳�
void CThreadPool::Init(CallBackFunc func, int count/* =10000 */)
{
    if(func != NULL)  
		m_fFunc = func;
    if(count > 0)
		m_nMaxThreadCount = count;
    m_hEvent = CreateEvent(0, TRUE, FALSE, 0); //�򿪻򴴽�һ���ֶ���λ������2TRUE���ĳ�ʼ״̬Ϊ���źţ�����3��������������4���¼�����
    m_hThread = CreateThread(0, 0, Thread, this, 0, &m_iThread);
    WaitForSingleObject(m_hEvent, -1);
    CloseHandle(m_hEvent);
}

//
DWORD WINAPI CThreadPool::Thread(LPVOID lp)
{
    CThreadPool *pThPool = (CThreadPool *)lp;
    DWORD dwCurId = GetCurrentThreadId();
    MSG msg;
    PeekMessage(&msg, 0, WM_USER, WM_USER, PM_NOREMOVE);
	//�������ܣ��ú���Ϊһ����Ϣ����߳���Ϣ���У���������Ϣ��������ڣ�����ָ���Ľṹ��
	//����ԭ�ͣ�BOOL PeekMessage��LPMSG IpMsg��HWND hWnd��UINT wMSGfilterMin��UINT wMsgFilterMax��UINT wRemoveMsg����
	//lpMsg��������Ϣ��Ϣ��MSG�ṹָ�롣
	//hWnd������Ϣ�����Ĵ��ڵľ����
	//wMsgFilterMin��ָ����������Ϣ��Χ��ĵ�һ����Ϣ��
	//wMsgFilterMax��ָ����������Ϣ��Χ������һ����Ϣ��
	//wRemoveMsg��ȷ����Ϣ��α������˲�����ȡ����ֵ֮һ��
	//PM_NOREMOVE��PeekMessage�������Ϣ���Ӷ����������
	//PM_REMOVE��PeekMessage�������Ϣ�Ӷ����������
	//�ɽ�PM_NOYIELD������ϵ�PM_NOREMOVE��PM_REMOVE���˱�־ʹϵͳ���ͷŵȴ����ó�����е��̡߳�
	//ȱʡ�أ������������͵���Ϣ����ֻ����ĳЩ��Ϣ��ָ��һ����������ֵ��
	//PM_QS_INPUT��Windows NT5.0��Windows 98���������ͼ�����Ϣ��
	//PM_QS_PAINT��Windows NT 5.0��Windows 98������ͼ��Ϣ��
	//PM_QS_POSTMESSAGE��Windows NT 5.0��Windows 98���������б����͵���Ϣ��������ʱ�����ȼ���
	//PM_QS_SENDMESSAGE��Windows NT 5.0��Windows 98���������з�����Ϣ��
	//����ֵ�������Ϣ�ɵõ������ط���ֵ�����û����Ϣ�ɵõ�������ֵ���㡣
    SetEvent(pThPool->m_hEvent);  //�����¼�Ϊ���ź�
    while (TRUE)
    {
        Sleep(1);

        if(PeekMessage(&msg, 0, WM_THREADSTART, WM_THREADPOOLIDIL, PM_REMOVE))
        {
            switch (msg.message)
            {
            case WM_THREADPOOLEND: 
				return true;
            case WM_THREADSTART: //�����߳�
            {
                spThread sTh = (spThread)msg.wParam;
                sTh->m_hThread = CreateThread(0, 0, sTh->m_fFunc, sTh->m_vParam, 0, 0);
                if(pThPool->m_fFunc) 
					pThPool->m_fFunc(WM_THREADSTART, sTh);
                break;
            }
            case WM_THREADEND: //�ر��߳�
            {
                spThread sTh = (spThread)msg.wParam;
                if(pThPool->m_fFunc)  pThPool->m_fFunc(WM_THREADEND, sTh);
                delete sTh;
                CloseHandle(sTh->m_hThread);
                break;
            }
            case WM_THREADADD:
            {//�����߳�---���߳̽ṹ����ȴ����е��߳�vector m_qWait��
                CThreadPool::spThread sTh = new CThreadPool::sThread;
                sTh->m_dRetval = 0;
                sTh->m_hThread = 0;
                sTh->m_fFunc = (ThreadFunc)msg.wParam;
                sTh->m_vParam = (LPVOID)msg.lParam;
                EnterCriticalSection(&(pThPool->m_cs));
                pThPool->m_qWait.push_back(sTh);
                LeaveCriticalSection(&(pThPool->m_cs));
                break;
            }
            case WM_THREADPOOLIDIL:
            {
                goto IDIL;
            }
            }
        }
        else
        {
IDIL:
            vector<CThreadPool::spThread> &vObj = pThPool->m_qRun;
            for(int i = 0; i < vObj.size(); i++)
            {
                DWORD iExitCode;
                GetExitCodeThread(vObj[i]->m_hThread, &iExitCode);
                if(iExitCode != STILL_ACTIVE)
                {
                    vObj[i]->m_dRetval = iExitCode;
                    PostThreadMessage(dwCurId, WM_THREADEND, (WPARAM)vObj[i], 0);
                    vObj.erase(vObj.begin() + i);
                    i--;
                }
            }
            if(vObj.size() < pThPool->m_nMaxThreadCount)
            {
                int nCount = pThPool->m_nMaxThreadCount - vObj.size();
                while(nCount)
                {
                    if(pThPool->m_qWait.size() > 0)
                    {
                        EnterCriticalSection(&(pThPool->m_cs));
                        vObj.push_back(pThPool->m_qWait[0]);
                        pThPool->m_qWait.erase(pThPool->m_qWait.begin());
                        LeaveCriticalSection(&(pThPool->m_cs));
                        PostThreadMessage(dwCurId, WM_THREADSTART, (WPARAM)vObj[vObj.size() - 1], 0);
                        while(1)
                        {
                            MSG msgtemp;
                            PeekMessage(&msgtemp, 0, WM_THREADSTART, WM_THREADPOOLIDIL, PM_NOREMOVE);
                            if(msgtemp.message != WM_THREADSTART)
                            {
                                PeekMessage(&msgtemp, 0, WM_THREADSTART, WM_THREADPOOLIDIL, PM_REMOVE);
                                PostThreadMessage(dwCurId, msgtemp.message, msgtemp.wParam, msgtemp.lParam);
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                    nCount--;
                }
            }

        }
    }
    return 0;
}

DWORD CThreadPool::GetRunThreadCount()
{
    return m_qRun.size();
}

CThreadPool::spThread CThreadPool::GetRunThreadItem(int nIndex)
{
    return m_qRun[nIndex];
}
DWORD CThreadPool::GetWaitThreadCount()
{
    return m_qWait.size();
}

CThreadPool::spThread CThreadPool::GetWaitThreadItem(int nIndex)
{
    return m_qWait[nIndex];
}

//�̺߳��������ǰ�߳�ID
DWORD WINAPI t(LPVOID l)
{
    cout << (int)l+1<<"��"<<GetCurrentThreadId() << endl;
    return 0;
}

DWORD WINAPI t1(LPVOID l)
{
    cout << "!!!" << endl;
    return 0;
}
void call(UINT m, CThreadPool::spThread sp)
{
    if(m == WM_THREADSTART)
    {
        //  cout<<1<<endl;
    }
}

int _tmain(int argc, _TCHAR *argv[])
{
    CThreadPool c;
    c.Init(0, 5);
    DWORD i = 1000;
    while(i--)
    {
        c.AddTask(t, (PVOID)i, 0);  //��һ��ִ��---�����߳�
    }
    c.AddTask(t1, 0, 1);

    getchar();
    return 0;
}

