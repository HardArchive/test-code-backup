/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   10:03
	file base:	CWindowIterator
	file ext:	h
	author:		zhangwf
	
	purpose:	封装枚举窗口类
*********************************************************************/
#ifndef _CWINDOW_ITERATOR_ZWF_H_
#define _CWINDOW_ITERATOR_ZWF_H_
//////////////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////
class CWindowIterator
{
public:
	// 构造析构函数
	CWindowIterator();
	~CWindowIterator();

public:
	// 接口函数
	void EnumWnds();                        // 枚举窗口
	unsigned long GetWndCounts();           // 获得窗口总数
	HWND First();                           // 获得第一个窗口句柄
	HWND Next();                            // 获得下一个窗口句柄
	HWND GetHWnd(unsigned long wndIndex);   // 根据索引获得窗口句柄		

protected:
	vector<HWND> m_hWndArray;               // 存放枚举得到的窗口句柄
	unsigned long m_nCurrentWndIndex;       // 当前窗口句柄

	// 枚举窗口回调函数
	static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lp);

	// virtual enumerator
	virtual BOOL AddWnd(HWND hwnd);

	// override to filter different kinds of windows
	virtual BOOL IsTargetWnd(HWND hwnd) {return TRUE;}

};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 派生出主窗口类（只获取进程的主窗口）
class CMainHWnd : public CWindowIterator
{
public:
	// 构造析构函数
	CMainHWnd();
	CMainHWnd(unsigned long pid);
	~CMainHWnd();

public:
	// 接口函数
	void EnumWndsByPid(DWORD pid);

protected:
	DWORD m_pid;
	virtual BOOL IsTargetWnd(HWND hwnd);

};
//////////////////////////////////////////////////////////////////////////
#endif