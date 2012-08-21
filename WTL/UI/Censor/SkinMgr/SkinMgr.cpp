/**************************************************************************
*   版权所有(C) 2010-2011 奔浪信息技术有限公司
*   文件：		SkinMgr.cpp
*   描述：		皮肤管理模块
*   作者：		ow
*   日期：		2011.12.28
**************************************************************************/
#include "stdafx.h"
#include "SkinMgr.h"
#include "SkinRes.h"

#define PROP_CLASSPTR	_T("PROP_CLASSPTR")

namespace SkinMgr
{

	BOOL SkinMgr_Initialize(HINSTANCE hInstance, HBITMAP hResBitmap)
	{
		ASSERT( hResBitmap != NULL );

		theInstance = hInstance;

		g_ResDC.CreateBitmapDC(hResBitmap);

		return TRUE;
	}

	BOOL SkinMgr_Free(void)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// CSkinWnd
	CSkinWnd::CSkinWnd(void)
	{
		m_hWnd = NULL;
		m_OrigWndProc = NULL;
	}

	CSkinWnd::~CSkinWnd(void)
	{
		Detach();
	}

	/*virtual*/ BOOL CSkinWnd::Attach(HWND hWnd)
	{
		ASSERT(m_hWnd == NULL && ::IsWindow(hWnd));
		m_hWnd = hWnd;
		
		m_OrigWndProc = (WNDPROC)(LONG_PTR)::GetWindowLong(m_hWnd, GWL_WNDPROC);

		::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)(WNDPROC)CSkinWnd::__WindowProc);
		SetProp(m_hWnd, PROP_CLASSPTR, (HANDLE)this);

		return TRUE;
	}

	/*virtual*/ BOOL CSkinWnd::Detach(void)
	{
		if ( ::IsWindow(m_hWnd) && NULL != m_OrigWndProc)
		{
			::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)(LONG_PTR)(WNDPROC)m_OrigWndProc);
			m_OrigWndProc = NULL;
		}

		m_OrigWndProc = NULL;
		m_hWnd = NULL;

		return TRUE;
	}

	/*virtual*/ LRESULT CSkinWnd::WindowProc(
		UINT uMsg, 
		WPARAM wParam, 
		LPARAM lParam)
	{
		ASSERT(m_OrigWndProc != NULL);

		return CallWindowProc(uMsg, wParam, lParam);
	}

	/*virtual*/ LRESULT CSkinWnd::CallWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_OrigWndProc, m_hWnd, uMsg, wParam, lParam);
	}

	/*static*/ LRESULT CALLBACK CSkinWnd::__WindowProc(
		HWND hwnd, 
		UINT uMsg, 
		WPARAM wParam, 
		LPARAM lParam)
	{
		CSkinWnd *pSkinWnd = (CSkinWnd*)GetProp(hwnd, PROP_CLASSPTR);
		if (pSkinWnd)
			return pSkinWnd->WindowProc(uMsg, wParam, lParam);
		else
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	HWND CSkinWnd::GetSafeHwnd(void) const
	{
		return m_hWnd;
	}

	HINSTANCE theInstance = NULL;
}