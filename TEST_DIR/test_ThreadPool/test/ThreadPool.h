

#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;
#define WM_THREADSTART    WM_USER+1
#define WM_THREADEND      WM_USER+2
#define WM_THREADADD      WM_USER+3
#define WM_THREADPOOLEND  WM_USER+4   //线程池结束
#define WM_THREADPOOLIDIL WM_USER+5
/***************************************************************************************************
* 1、 Class      ： CThreadPool.c
* 2、 Version    ： *.*
* 3、 Description： 
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-10-17 10:08:32
* 6、 History    ： 
* 7、 Remark     ： 这个类的内部定义了两个vector，一个是储存当前运行的线程，还有一个是储存等待运行的线程，
*				    这个由m_nMaxThreadCount决定，当我们init线程池的时候，有一个PeekMessage(&msg,0,WM_USER,WM_USER,PM_NOREMOVE);语句，
*				    会强制创建消息队列。
*				    然后我们便可以在线程的消息队列中接受消息来控制线程的运行，
*				    当idil时，线程池会自动移除当前运行vector里终止的线程，从等待vector里移入待运行的线程，
*				    并提高WM_THREADSTART消息的优先级，确保新线程能即使创建
****************************************************************************************************/
class CThreadPool
{
public:
	typedef DWORD (WINAPI *ThreadFunc)(LPVOID);
	//线程结构
	typedef struct _sThread
	{
		HANDLE m_hThread;    //线程句柄
		ThreadFunc m_fFunc;  //线程执行函数指针
		void    *m_vParam;   //线程参数
		DWORD  m_dRetval;    //GetExitCodeThread取得的线程退出代码
	} sThread, *spThread;
	typedef void (*CallBackFunc)(UINT, spThread);

	CThreadPool();
	~CThreadPool();

	void AddTask(ThreadFunc, LPVOID, bool = 0 ); //增加等待处理线程
	void Init(CallBackFunc, int = 10000);   //初始化线程池
	DWORD GetRunThreadCount();         //取得当前运行线程数量
	spThread GetRunThreadItem(int);    //得到当前运行线程结构
	DWORD GetWaitThreadCount();        //取得等待线程数量
	spThread GetWaitThreadItem(int);   //取得等待运行线程结构

private:
	static DWORD WINAPI Thread(LPVOID);  //管理线程实现函数 
	DWORD m_iThread;
	HANDLE m_hThread;          //管理线程句柄
	CallBackFunc m_fFunc;
	DWORD m_nMaxThreadCount;   //最大线程数量
	vector<spThread> m_qRun;   //当前运行线程

	vector<spThread> m_qWait;  //等待中的线程
	HANDLE m_hEvent;
	CRITICAL_SECTION m_cs;     //临界区对象
};
