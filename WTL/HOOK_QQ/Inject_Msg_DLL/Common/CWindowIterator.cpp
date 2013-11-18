/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   10:10
	file base:	CWindowIterator
	file ext:	cpp
	author:		zhangwf
	
	purpose:	封装枚举窗口类
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <Windows.h>
#include "CWindowIterator.h"
//////////////////////////////////////////////////////////////////////////
// 构造函数
CWindowIterator::CWindowIterator()
{
	// 初始化变量
	m_hWndArray.clear();
	m_nCurrentWndIndex = 0;
}

// 析构函数
CWindowIterator::~CWindowIterator()
{

}

//////////////////////////////////////////////////////////////////////////
// 接口函数
// 获得窗口总数
unsigned long CWindowIterator::GetWndCounts()
{
	return m_hWndArray.size();
}

// 获得第一个窗口句柄
HWND CWindowIterator::First()
{
	if (m_hWndArray.size() != 0)
	{
		m_nCurrentWndIndex = 0;
		return m_hWndArray[m_nCurrentWndIndex];
	}

	return NULL;
}

// 获得下一个窗口句柄
HWND CWindowIterator::Next()
{
	if (m_nCurrentWndIndex+1 < m_hWndArray.size())
	{
		m_nCurrentWndIndex += 1;
		return m_hWndArray[m_nCurrentWndIndex];
	}

	return NULL;
}

// 根据索引获得窗口句柄
HWND CWindowIterator::GetHWnd(unsigned long wndIndex)
{
	if (wndIndex < m_hWndArray.size())
	{
		return m_hWndArray[wndIndex];
	}

	return NULL;
}

void CWindowIterator::EnumWnds()
{
	// 清除上次内容
	m_hWndArray.clear();

	// 枚举所有窗口
	::EnumWindows(EnumWndProc, (LPARAM)this);
}

//////////////////////////////////////////////////////////////////////////
// 非接口函数
// 枚举窗口回调函数
BOOL CALLBACK CWindowIterator::EnumWndProc(HWND hwnd, LPARAM lp)
{
	return ((CWindowIterator*)lp)->AddWnd(hwnd);
}

// virtual enumerator
BOOL CWindowIterator::AddWnd(HWND hwnd)
{
	if (IsTargetWnd(hwnd) == TRUE)
	{
		m_hWndArray.push_back(hwnd);		
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 派生出主窗口句柄类
// 构造函数
CMainHWnd::CMainHWnd()
:CWindowIterator()
{
	m_pid = 0;
}

// 带参构造函数
CMainHWnd::CMainHWnd(unsigned long pid)
:CWindowIterator()
{
	m_pid = pid;
	EnumWnds();
}

// 析构函数
CMainHWnd::~CMainHWnd()
{

}

//////////////////////////////////////////////////////////////////////////
// 接口函数
void CMainHWnd::EnumWndsByPid(DWORD pid)
{
	m_pid = pid;
	EnumWnds();
}
//////////////////////////////////////////////////////////////////////////
// 非接口函数
BOOL CMainHWnd::IsTargetWnd(HWND hwnd)
{
	// 顶层且可见
	if (::GetParent(hwnd)==NULL && ::IsWindowVisible(hwnd)==TRUE) 
	{		
		DWORD pidwin;
		::GetWindowThreadProcessId(hwnd, &pidwin);
		if (pidwin == m_pid)
		{
			
			return TRUE;
		}			
	}

	// 不是目标窗口
	return FALSE;
}