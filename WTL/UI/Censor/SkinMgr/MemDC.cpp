#include "StdAfx.h"
#include "SkinMgr.h"

namespace SkinMgr
{
	CMemDC::CMemDC(void)
	{
		m_hMemBitmap = NULL;
		m_hOldBitmap = NULL;

		m_Width = m_Height = 0;
	}

	CMemDC::~CMemDC(void)
	{
		DeleteMemDC();
	}

	BOOL CMemDC::CreateBitmapDC(HBITMAP hBitmap)
	{
		if (NULL != m_hDC) DeleteMemDC();

		HDC hTmpDC = ::GetDC((HWND)0);
		if (NULL == hTmpDC) return FALSE;

		HDC hMemDC = ::CreateCompatibleDC(hTmpDC);
		if (NULL == hTmpDC)
		{
			::ReleaseDC((HWND)0, hTmpDC);
			return FALSE;
		}

		m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, (HGDIOBJ)hBitmap);
		m_hMemBitmap = hBitmap;

		BITMAP bm;
		::GetObject(hBitmap, sizeof(BITMAP), &bm);

		m_Width = bm.bmWidth;
		m_Height = bm.bmHeight;

		::ReleaseDC((HWND)0, hTmpDC);

		m_hDC = hMemDC;

		return TRUE;
	}

	BOOL CMemDC::CreateMemDC(int cx, int cy)
	{
		if (NULL != m_hDC) DeleteMemDC();

		HDC hTmpDC = ::GetDC((HWND)0);
		if (NULL == hTmpDC) return FALSE;

		HDC hMemDC = ::CreateCompatibleDC(hTmpDC);
		if (NULL == hTmpDC)
		{
			::ReleaseDC((HWND)0, hTmpDC);
			return FALSE;
		}

		HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hTmpDC, cx, cy);
		if (NULL == hMemBitmap)
		{
			::DeleteDC(hMemDC);
			::ReleaseDC((HWND)0, hTmpDC);
			return FALSE;
		}

		m_hOldBitmap = (HBITMAP)::SelectObject(hMemDC, (HGDIOBJ)hMemBitmap);
		m_hMemBitmap = hMemBitmap;

		::ReleaseDC((HWND)0, hTmpDC);

		m_hDC = hMemDC;

		m_Width = cx;
		m_Height = cy;

		return TRUE;
	}

	BOOL CMemDC::DeleteMemDC(void)
	{
		if (NULL == m_hDC) return FALSE;

		if (m_hOldBitmap)
		{
			::SelectObject(m_hDC, m_hOldBitmap);
			m_hOldBitmap = NULL;
		}

		if (m_hMemBitmap)
		{
			::DeleteObject(m_hMemBitmap);
			m_hMemBitmap = NULL;
		}

		m_Width = m_Height = 0;

		return DeleteDC();
	}

	BOOL CMemDC::GridBlt(int x, int y, int nWidth, int nHeight, 
		HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
		int nL, int nT, int nR, int nB, DWORD dwRop)
	{
		ATLASSERT(NULL != m_hDC);

		// ◊Û…œΩ«
		::BitBlt(m_hDC, x, y, nL, nT, hSrcDC, xSrc, ySrc, dwRop);
		// ”“…œΩ«
		::BitBlt(m_hDC, x + nWidth - nL, y, nR, nT, hSrcDC, xSrc + nSrcWidth - nL, ySrc, dwRop);
		// ◊Ûœ¬Ω«
		::BitBlt(m_hDC, x, y + nHeight - nB, nL, nB, hSrcDC, xSrc, ySrc + nSrcHeight - nB, dwRop);
		// ”“œ¬Ω«
		::BitBlt(m_hDC, x + nWidth - nR, y + nHeight - nB, nR, nB, hSrcDC, xSrc + nSrcWidth - nR, ySrc + nSrcHeight - nB, dwRop);

		// ∂•≤ø
		::StretchBlt(m_hDC, x + nL, y, nWidth - nL - nR, nT, hSrcDC, xSrc + nL, ySrc, nSrcWidth - nL - nR, nT, dwRop);
		// ◊Û≤‡
		::StretchBlt(m_hDC, x, y + nT, nL, nHeight - nT - nB, hSrcDC, xSrc, ySrc + nT, nL, nSrcHeight - nT - nB, dwRop);
		// ”“≤‡
		::StretchBlt(m_hDC, x + nWidth - nR, y + nT, nR, nHeight - nT - nB, hSrcDC, xSrc + nSrcWidth - nR, ySrc + nT, nR, nSrcHeight - nT - nB, dwRop);
		// µ◊≤ø
		::StretchBlt(m_hDC, x + nL, y + nHeight - nB, nWidth - nL - nR, nB, hSrcDC, xSrc + nL, ySrc + nSrcHeight - nB, nSrcWidth - nL - nR, nB, dwRop);

		// ÷–º‰…Ï’π
		::StretchBlt(m_hDC, x + nL, y + nT, nWidth - nL - nR, nHeight - nT - nB, hSrcDC, xSrc + nL, ySrc + nT, nSrcWidth - nL - nR, nSrcHeight - nT - nB, dwRop);
		return TRUE;
	}

	BOOL CMemDC::FillColor(int x, int y, int cx, int cy, COLORREF clr)
	{
		HBRUSH hBrush = ::CreateSolidBrush(clr);
		RECT rc;
		SetRect(&rc, x, y, x + cx, y + cy);
		BOOL bReturn = FillRect(&rc, hBrush);
		::DeleteObject((HGDIOBJ)hBrush);
		return bReturn;
	}

	int CMemDC::GetWidth(void) const
	{
		return m_Width;
	}

	int CMemDC::GetHeight(void) const
	{
		return m_Height;
	}
}