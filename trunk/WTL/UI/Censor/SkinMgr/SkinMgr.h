/**************************************************************************
*   版权所有(C) 2010-2011 奔浪信息技术有限公司
*   文件：		SkinMgr.h
*   描述：		皮肤管理模块
*   作者：		ow
*   日期：		2011.12.28
**************************************************************************/
#pragma once

#define BUTTON_COUNT 4
#define SKINMGR_TCN_SELCHANGE (TCN_FIRST - 1)
#define SKINMGR_TCN_LBUTTONDBLCLK (TCN_FIRST - 2)
#define SKINMGR_TCN_LBUTTONDOWN (TCN_FIRST - 3)

namespace SkinMgr
{
	BOOL SkinMgr_Initialize(HINSTANCE hInstance, HBITMAP hResBitmap);
	BOOL SkinMgr_Free(void);

	class CMemDC : public CDC
	{
	public:
		CMemDC(void);
	public:
		~CMemDC(void);
	public:
		BOOL CreateBitmapDC(HBITMAP hBitmap);
		BOOL CreateMemDC(int cx, int cy);
		BOOL DeleteMemDC(void);
		BOOL GridBlt(int x, int y, int nWidth, int nHeight, 
			HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
			int gx1, int gy1, int gx2, int gy2, DWORD dwRop);
		BOOL FillColor(int x, int y, int cx, int cy, COLORREF clr);
		int GetWidth(void) const;
		int GetHeight(void) const;
	private:
		HBITMAP m_hMemBitmap;
		HBITMAP m_hOldBitmap;
		int m_Width;
		int m_Height;
	};


	class CSkinWnd
	{
	public:
		CSkinWnd(void);
		~CSkinWnd(void);
	public:
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL Detach(void);
		virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT CallWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK __WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HWND GetSafeHwnd(void) const;
	protected:
		HWND m_hWnd;
		WNDPROC m_OrigWndProc;
	};

	class CSkinFrame : public CSkinWnd
	{
	public:
		CSkinFrame(void);
		~CSkinFrame(void);

	public:
		UINT HitTest(int x, int y);
		BOOL Attach(HWND hWnd);
		BOOL Detach(void);
		void SetMinSize(int nMinWidth, int nMinHeight);
		void SetMinWidth(int nMinWidth);
		int GetMinWidth(void) const;
		void SetMinHeight(int nMinHeight);
		int GetMinHeight(void) const;
	protected:
		BOOL DrawSkin(BOOL bUpdate = FALSE);
		BOOL DrawButton(BOOL bUpdate = FALSE);
		BOOL SetWindowRgn(void);
		void TrackSystemMenu(void);
		virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		static RECT rcIcon;
	protected:
		LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnMouseDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnMouseDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnMouseUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
	protected:
		CMemDC m_MemDC;
		CMemDC m_ButtonDC;
		DWORD	m_ButtonState[BUTTON_COUNT];	// 标题栏按钮状态
		int		m_CurHoverButton;	// 分别是标题栏按钮高亮和按下的标记
		int		m_CurPushButton;
		int		m_nMinWidth;	// 分别是最小宽度和高度
		int		m_nMinHeight;
		
		HFONT m_hCapFont;	// 标题栏字体
	};

	typedef struct tagSKINTABITEM
	{
		LPTSTR	lpszCaption;
		HICON	hIcon;
		RECT	rc;
	}SKINTABITEM, *PSKINTABITEM;

	typedef struct tagSKINTABNOTIFY
	{
		NMHDR hdr;
		int Index;
	}SKINTABNOTIFY, *LPSKINTABNOTIFY;

	class CSkinTabCtrl : public CSkinWnd
	{
	public:
		CSkinTabCtrl(void);
		~CSkinTabCtrl(void);
	public:
		BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpWindowName, 
			DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent = NULL, UINT nID = 0);
		virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL AddItem(LPCTSTR lpszCaption, HICON hIcon = 0);
		int SetSelected(int Index);
	protected:
		BOOL DrawSkin(BOOL bUpdate = FALSE);
		int HitTest(int x, int y);
		LRESULT SendNotifyMessage(UINT code, int Index);
		BOOL PreRegisterClass(LPCTSTR lpszClassName);
	protected:
		LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
		LRESULT OnMouseMoveLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

	protected:
		PSKINTABITEM m_pItems;
		UINT m_ItemCount;
		int m_CurSelected;
		int m_CurHorver;
		HFONT m_hFont;

		CMemDC m_MemDC;
	};

	extern HINSTANCE theInstance;
}