//Download by http://www.codefans.net

#include "StdAfx.h"
#include <winuser.h>
#include <Windows.h>
#include <Windowsx.h>
#include <cmath>
#include "DlgBase.h"
#include "SystemButton.h"
#include "NormalButton.h"
#include "ImageButton.h"
#include "Area.h"
#include "SolelyButton.h"
#include "StopwatchTimer.h"


#define FRAME_X_BK_BITMAP "C:\\FrameX_BK.bmp"

char g_cMinimize[20][20] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

char g_cMaximize[20][20] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

char g_cRestore[20][20] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

char g_cClose[20][20] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
							{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

IMPLEMENT_DYNAMIC(CDlgBase, CDialog)

CDlgBase::CDlgBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
{
	m_FrameRect.SetRect(0,0,0,0);
	m_MinSize.cx = 640;
	m_MinSize.cy = 480;

	m_nFrameTopBottomSpace = 3;
	m_nFrameLeftRightSpace = 3;

	m_nOverRegio = 100;
	m_sizeCustomBKImage.cx = 100;
	m_sizeCustomBKImage.cy = 100;

	
	m_hIcon = NULL;
	m_bIsImageStyle = false;
	m_bTracking = false;
	m_bIsSetCapture = false;
	m_clrCustomBK = RGB(186, 226, 239);

	m_bIsLButtonDown = FALSE;
	m_rcLButtonDown.SetRect(-1, -1, -1, -1);
	m_bIsLButtonDblClk = FALSE;
	m_pOldMemBK = NULL;
	m_pControl = NULL;

	char fileName[MAX_PATH];
	if (!GetModuleFileName(NULL, fileName, sizeof(fileName)))
		return ;

	char *p = strrchr(fileName, '\\');
	if (p)
	{
		*(p + 1) = '\0';
	}

	m_strPath = fileName;
}

CDlgBase::~CDlgBase()
{
	if (m_hIcon)
	{
		DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}
	if(m_defaultBKImage.m_hObject != NULL)
	{
		m_defaultBKImage.DeleteObject();
	}

	if(m_CustomBKImage.m_hObject != NULL)
	{
		m_CustomBKImage.DeleteObject();
		m_MemBKDC.SelectObject(m_pOldMemBK);
		m_MemBK.DeleteObject();
	}

	if(m_TitleFont.m_hObject != NULL)
	{
		m_TitleFont.DeleteObject();
	}
	m_Rgn.DeleteObject();

	for (int i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}

	for (int i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			delete pControlBase;
		}		
	}
}

BEGIN_MESSAGE_MAP(CDlgBase, CDialog)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_NCACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_SYSCOMMAND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// 设置最小窗口大小
void CDlgBase::SetMinSize(int iWidth, int iHeight)
{
	m_MinSize.cx = iWidth;
	m_MinSize.cy = iHeight;
}

SIZE CDlgBase::GetMinSize()
{
	return	m_MinSize;
}

void CDlgBase::GetFrameRect(RECT* pRect)
{
	*pRect = m_FrameRect;
}

void CDlgBase::SetIcon(HICON hIcon, BOOL bBigIcon)
{
	if (bBigIcon == 0)
	{
		if (m_hIcon)
		{
			DestroyIcon(m_hIcon);
			m_hIcon = NULL;
		}
		m_hIcon = CopyIcon(hIcon);
	}

	//	HICON hOldIcon = CDialog::SetIcon(hIcon, bBigIcon);
	HICON hOldIcon = (HICON)SendMessage(WM_SETICON,
		bBigIcon ? ICON_BIG : ICON_SMALL, (LPARAM)m_hIcon);
	if (hOldIcon)
	{
		DestroyIcon(hOldIcon);
	}
}

bool CDlgBase::SetIcon(int IID, BOOL bBigIcon)
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(IID), RT_GROUP_ICON);
	if (hRes)
	{
		HGLOBAL hGlobal = LoadResource(hModule, hRes);
		if (hGlobal)
		{
			LPBYTE pData = (LPBYTE)LockResource(hGlobal);
			DWORD dwSize = SizeofResource(hModule, hRes);

			int iIconSize = 16;
			if (bBigIcon)
			{
				iIconSize = 32;
			}

			int nID = LookupIconIdFromDirectoryEx(pData, TRUE,
				iIconSize, iIconSize, LR_DEFAULTCOLOR);

			FreeResource(hGlobal);

			if (nID > 0)
			{
				hRes = FindResource(hModule, MAKEINTRESOURCE(nID), RT_ICON);

				if (hRes)
				{
					hGlobal = LoadResource(hModule, hRes);

					if (hGlobal)
					{
						pData = (LPBYTE)LockResource(hGlobal);
						dwSize = SizeofResource(hModule, hRes);

						HICON hIcon = CreateIconFromResourceEx(
							pData, dwSize, TRUE,
							0x00030000, iIconSize, iIconSize, LR_DEFAULTCOLOR);

						FreeResource(hGlobal);

						if (hIcon)
						{
							SetIcon(hIcon, bBigIcon);
							DestroyIcon(hIcon);
							return	true;
						}
					}
				}
			}
		}
	}

	HICON hIcon = AfxGetApp()->LoadIcon(IID);
	if(hIcon)
	{
		SetIcon(hIcon, bBigIcon);
		DestroyIcon(hIcon);
		return true;
	}
	return	false;
}

void CDlgBase::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = m_MinSize.cx;
	lpMMI->ptMinTrackSize.y = m_MinSize.cy;

	CRect	rc(0, 0, 1024, 768);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0); // 取得桌面区域
	lpMMI->ptMaxPosition.x = rc.left;
	lpMMI->ptMaxPosition.y = rc.top;
	lpMMI->ptMaxSize.x = rc.Width();
	lpMMI->ptMaxSize.y = rc.Height();
}

// CDlgBase message handlers

BOOL CDlgBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect	rc;
	GetClientRect(rc);

	CFont	tmpFont;
	tmpFont.Attach(GetStockObject(DEFAULT_GUI_FONT));

	LOGFONT font;
	memset(&font, 0, sizeof(font));
	tmpFont.GetLogFont(&font);

	CWindowDC dc(this);
	strcpy(font.lfFaceName,_T("宋体"));
	font.lfHeight = -10 * GetDeviceCaps(dc.m_hDC, LOGPIXELSY) / 72;
	font.lfWeight = 600;

	m_TitleFont.CreateFontIndirect(&font);

	SetWindowPos(NULL, 0, 0, m_MinSize.cx, m_MinSize.cy, SWP_NOMOVE);

	InitUI();

	return TRUE;
}

void CDlgBase::InitUI()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcTemp;
	CControlBase * pControlBase = NULL;

  	rcTemp.SetRect(0, 44, rcClient.right, 88);
  	pControlBase = new CArea(GetSafeHwnd(), -2, rcTemp, 85, 15);
  	m_vecArea.push_back(pControlBase);
  
   	rcTemp.SetRect(0, 88, rcClient.right, rcClient.bottom);
   	pControlBase = new CArea(GetSafeHwnd(), -1, rcTemp, 15, 15);
   	m_vecArea.push_back(pControlBase);
   
   	rcTemp.SetRect(0, rcClient.bottom - 33, rcClient.right, rcClient.bottom);
   	pControlBase = new CArea(GetSafeHwnd(), 0, rcTemp, 85, 85, m_clrCustomBK);
   	m_vecArea.push_back(pControlBase);



	int nStartButton = rcClient.Width() - 39;
	rcTemp.SetRect(nStartButton, -2, nStartButton + 39, 18);
	pControlBase = new CImageButton(GetSafeHwnd(), 1, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(m_strPath + "SKIN\\CloseNormal.png", m_strPath + "SKIN\\CloseHover.png",
		m_strPath + "SKIN\\CloseDown.png", m_strPath + "SKIN\\CloseDisable.png");
	m_vecControl.push_back(pControlBase);

 	nStartButton -= 28;
 	rcTemp.SetRect(nStartButton, -2, nStartButton + 28, 18);
	pControlBase = new CImageButton(GetSafeHwnd(), 2, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(m_strPath + "SKIN\\MaxNormal.png", m_strPath + "SKIN\\MaxHover.png",
		m_strPath + "SKIN\\MaxDown.png");
 	m_vecControl.push_back(pControlBase);

	rcTemp.SetRect(nStartButton, -2, nStartButton + 28, 18);
	pControlBase = new CImageButton(GetSafeHwnd(), 3, rcTemp, "", FALSE);
	((CImageButton *)pControlBase)->SetBitmap(m_strPath + "SKIN\\RestoreNormal.png", m_strPath + "SKIN\\RestoreHover.png",
		m_strPath + "SKIN\\RestoreDown.png");
	m_vecControl.push_back(pControlBase);

	nStartButton -= 28;
	rcTemp.SetRect(nStartButton, -2, nStartButton + 28, 18);
	pControlBase = new CImageButton(GetSafeHwnd(), 4, rcTemp);
	((CImageButton *)pControlBase)->SetBitmap(m_strPath + "SKIN\\MiniNormal.png", m_strPath + "SKIN\\MiniHover.png",
		m_strPath + "SKIN\\MiniDown.png");
	m_vecControl.push_back(pControlBase);


	nStartButton = rcClient.right - 100;
	rcTemp.SetRect(nStartButton, rcClient.bottom - 29,nStartButton + 86, rcClient.bottom - 5);
	pControlBase = new CImageButton(GetSafeHwnd(), 5, rcTemp, "取消");
	((CImageButton *)pControlBase)->SetBitmap(m_strPath + "SKIN\\Normal.png", m_strPath + "SKIN\\Hover.png",
		m_strPath + "SKIN\\Down.png", m_strPath + "SKIN\\Disable.png");
	m_vecControl.push_back(pControlBase);
  
// 	rcTemp.SetRect(nStartButton, rcClient.bottom - 80, nStartButton + 86, rcClient.bottom - 50);
// 	pControlBase = new CNormalButton(GetSafeHwnd(), 8, rcTemp, m_strPath + "SKIN\\NormalButton.bmp");
// 	m_vecControl.push_back(pControlBase);

	nStartButton -= 100;
  	rcTemp.SetRect(nStartButton, rcClient.bottom - 29,nStartButton + 86, rcClient.bottom - 5);
  	pControlBase = new CImageButton(GetSafeHwnd(), 6, rcTemp, "确定", TRUE);
   	((CImageButton *)pControlBase)->SetBitmap(m_strPath + "SKIN\\Normal.png", m_strPath + "SKIN\\Hover.png",
   		m_strPath + "SKIN\\Down.png", m_strPath + "SKIN\\Disable.png");
  	m_vecControl.push_back(pControlBase);

// 	rcTemp.SetRect(nStartButton, rcClient.bottom - 80, nStartButton + 86, rcClient.bottom - 50);
// 	pControlBase = new CNormalButton(GetSafeHwnd(), 7, rcTemp, m_strPath + "SKIN\\NormalButton.bmp");
// 	m_vecControl.push_back(pControlBase);


	nStartButton = 10;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 10, rcTemp, m_strPath + "SKIN\\0.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 11, rcTemp, m_strPath + "SKIN\\1.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 12, rcTemp, m_strPath + "SKIN\\2.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 13, rcTemp, m_strPath + "SKIN\\3.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 14, rcTemp, m_strPath + "SKIN\\4.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 15, rcTemp, m_strPath + "SKIN\\5.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 16, rcTemp, m_strPath + "SKIN\\6.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 17, rcTemp, m_strPath + "SKIN\\7.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 18, rcTemp, m_strPath + "SKIN\\8.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

	nStartButton += 48;
	rcTemp.SetRect(nStartButton, 40, nStartButton + 48, 104);
	pControlBase = new CSolelyButton(GetSafeHwnd(), 19, rcTemp, m_strPath + "SKIN\\9.png");
	pControlBase->SetTiemrIsRun(true);
	m_vecControl.push_back(pControlBase);

// 	rcTemp.SetRect(0, 300, 1000, 500);
// 	pControlBase = new CStopwatchTimer(GetSafeHwnd(), 20, rcTemp);
// 	pControlBase->SetTiemrIsRun(true);
// 	((CStopwatchTimer *)pControlBase)->SetImage(m_strPath + "SKIN\\");
// 	((CStopwatchTimer *)pControlBase)->StartTimer();
// 	m_vecControl.push_back(pControlBase);

}

CControlBase *CDlgBase::GetControl(UINT uControlID)
{
	for (int i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetControlID() == uControlID)
			{
				return pControlBase;
			}
		}
	}
	return NULL;
}

int CDlgBase::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 设置窗口风格
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE)
		| WS_MAXIMIZEBOX | WS_MINIMIZEBOX
		| WS_SYSMENU | WS_SIZEBOX;
	dwStyle &= ~(WS_CAPTION);

	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	if (!m_bIsImageStyle)
	{
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, FRAME_X_BK_BITMAP, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBmp)
		{
			m_defaultBKImage.Attach(hBmp);
		}
	}
	else
	{
		BOOL bIsCustomBKImage = false;
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, m_strPath + "SKIN\\BK.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBmp)
		{
			m_CustomBKImage.Attach(hBmp);
			BITMAP bmInfo;
			::GetObject( m_CustomBKImage.m_hObject, sizeof(BITMAP), &bmInfo );
			m_sizeCustomBKImage.cx = bmInfo.bmWidth;
			m_sizeCustomBKImage.cy = bmInfo.bmHeight;
			bIsCustomBKImage = TRUE;					
		}

		DrawBKImage(bIsCustomBKImage);	
	}

	return CDialog::OnCreate(lpCreateStruct);
}
void CDlgBase::DrawBKImage(BOOL bIsCustomBKImage)
{
	DWORD dwStart = GetTickCount();

	CDC *pDC = GetDC();

	if (bIsCustomBKImage)
	{
		m_MemBKDC.CreateCompatibleDC(pDC);

		m_MemBK.CreateCompatibleBitmap(pDC, m_sizeCustomBKImage.cx, m_sizeCustomBKImage.cy);
		m_pOldMemBK =  m_MemBKDC.SelectObject(&m_MemBK);

		CDC TempDC;
		TempDC.CreateCompatibleDC(pDC);

		CBitmap* pOldBitmap = TempDC.SelectObject(&m_CustomBKImage);

		long r = 0, g = 0, b = 0;
		COLORREF clrTemp;

		//计算平均颜色
		for(int i = 0; i < m_sizeCustomBKImage.cx; i++)			
		{
			for(int j = 0; j < m_sizeCustomBKImage.cy; j++)
			{
				clrTemp = TempDC.GetPixel(i, j);
				r += BYTE( GetRValue(clrTemp));
				g += BYTE( GetGValue(clrTemp));
				b += BYTE( GetBValue(clrTemp));
			}
		}

		int n = m_sizeCustomBKImage.cx  * m_sizeCustomBKImage.cy;
		m_clrCustomBK = RGB(r / n, g / n, b / n);

		DWORD dwEnd = GetTickCount();

		TRACE("计算图片颜色所用时间： %d(ms)[%d * %d]RGB(%d, %d, %d)\n", dwEnd - dwStart, m_sizeCustomBKImage.cx, m_sizeCustomBKImage.cy,
			BYTE( GetRValue(m_clrCustomBK)), BYTE( GetGValue(m_clrCustomBK)), BYTE( GetBValue(m_clrCustomBK)));

		dwStart = dwEnd;

		//画出平均图片
		int nWidth = m_sizeCustomBKImage.cx;
		int nHeight = m_sizeCustomBKImage.cy;
		BLENDFUNCTION blend;
		memset( &blend, 0, sizeof( blend) );
		blend.BlendOp= AC_SRC_OVER;
		blend.SourceConstantAlpha = 255; // 透明度	
		int nStartPosition = 0;


		//左上
		nStartPosition = m_sizeCustomBKImage.cx - m_nOverRegio;
		m_MemBKDC.BitBlt(0, 0, nStartPosition , m_sizeCustomBKImage.cy - m_nOverRegio, &TempDC, 0, 0, SRCCOPY ); 

		//上中
		m_MemBKDC.FillSolidRect(nStartPosition, 0, m_nOverRegio, m_sizeCustomBKImage.cy - m_nOverRegio, m_clrCustomBK); 

		//下中
		nStartPosition = m_sizeCustomBKImage.cy - m_nOverRegio;
		m_MemBKDC.FillSolidRect(0, nStartPosition, m_sizeCustomBKImage.cx - m_nOverRegio, m_nOverRegio, m_clrCustomBK); 

		//中间
		DrawBKImageCross(m_MemBKDC, TempDC);

		//上中
		nStartPosition = m_sizeCustomBKImage.cx - m_nOverRegio;
		for (int i = 0; i < m_nOverRegio; i++)
		{
			blend.SourceConstantAlpha = 255 - (2.55 * i); 
			m_MemBKDC.AlphaBlend(nStartPosition + i, 0, 1 , m_sizeCustomBKImage.cy - m_nOverRegio, 
				&TempDC, nStartPosition + i, 0, 1 , m_sizeCustomBKImage.cy - m_nOverRegio, blend); 
		}

		//下中
		CRect rcTemp(0, m_sizeCustomBKImage.cy - m_nOverRegio, m_sizeCustomBKImage.cx - m_nOverRegio, m_sizeCustomBKImage.cy);
		DrawVerticalTransition(m_MemBKDC, TempDC, rcTemp);

		//nStartPosition = m_sizeCustomBKImage.cy - m_nOverRegio;
		//for (int i = 0; i < m_nOverRegio; i++)
		//{
		//	blend.SourceConstantAlpha = 255 - (2.55 * i); 
		//	m_MemBKDC.AlphaBlend(0, nStartPosition + i, m_sizeCustomBKImage.cx - m_nOverRegio , 1, 
		//		&TempDC, 0, nStartPosition + i, m_sizeCustomBKImage.cx - m_nOverRegio, 1, blend); 
		//}
		TempDC.SelectObject(pOldBitmap);
		TempDC.DeleteDC();
	}
	else
	{
		pDC->FillSolidRect(0, 0, 100, 100, m_clrCustomBK);
	}
	DWORD dwDrawEnd = GetTickCount();
	TRACE("绘背景图所用时间： %d(ms)[%d * %d]\n", dwDrawEnd - dwStart, m_sizeCustomBKImage.cx, m_sizeCustomBKImage.cy);
	

	ReleaseDC(pDC);

}

BOOL CDlgBase::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


bool CDlgBase::IsFocus()
{
	return ::GetFocus() == m_hWnd 
		|| ::GetAncestor(::GetFocus(), 2) == m_hWnd;
}

void CDlgBase::OnPaint()
{
	GetUpdateRect(&m_rcUpdate, FALSE);
	CPaintDC	dc(this);
	CRect	rcClient;
	
	dc.SetBkMode(TRANSPARENT);
	GetClientRect(rcClient);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap *pOldmap =  MemDC.SelectObject(&memBmp);

	DrawImageStyle(MemDC, rcClient);

	//dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);
	dc.BitBlt(m_rcUpdate.left, m_rcUpdate.top, m_rcUpdate.Width(), m_rcUpdate.Height(), &MemDC, m_rcUpdate.left, m_rcUpdate.top, SRCCOPY);

	MemDC.SelectObject(pOldmap);
	MemDC.DeleteDC();
}


void CDlgBase::DrawImageStyle(CDC &dc, const CRect &rcClient)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SetBkMode(TRANSPARENT);
	  
 // 	// 图标
 // 	if (m_hIcon)
 // 	{
 // 		DrawIconEx(dc.m_hDC, 9, (m_nFrameTopSpace - 16) / 2, m_hIcon, 16, 16, 0, NULL, DI_NORMAL);
 // 	}
 // 	else
 // 	{
 // 		HICON hIcon = (HICON)GetIcon(FALSE);
 // 		if (hIcon)
 // 		{
 // 			DrawIconEx(MemDC.m_hDC, 9, (m_nFrameTopSpace - 16) / 2, hIcon, 16, 16, 0, NULL, DI_NORMAL);
 // 		}
 // 		else
 // 		{
 // 			hIcon = (HICON)::LoadIcon(NULL, IDI_APPLICATION);
 // 			DrawIconEx(MemDC.m_hDC, 9, (m_nFrameTopSpace - 16) / 2, hIcon, 16, 16, 0, NULL, DI_NORMAL);
 // 			DestroyIcon(hIcon);
 // 		}
 // 	}
 // 

	dc.FillSolidRect(0, 0, rcClient.Width(), rcClient.Height(), m_clrCustomBK);

	int nShowBkImageWidth = rcClient.Width();
	int nShowBkImageHeight = rcClient.Height();

	
	if (rcClient.Width() > m_sizeCustomBKImage.cx)
	{
		//右
		nShowBkImageWidth = m_sizeCustomBKImage.cx;
	}

	if (rcClient.Height() > m_sizeCustomBKImage.cy)
	{
		//下
		nShowBkImageHeight = m_sizeCustomBKImage.cy;
	}

	//左上
	dc.BitBlt(0, 0, nShowBkImageWidth , nShowBkImageHeight, &m_MemBKDC, 0, 0, SRCCOPY ); 

	//画控件
	DrawControl(dc, rcClient);

	CRect rcClientTemp = rcClient;
	//画边框
	if (!IsZoomed())
	{
		CDC TempDC;
		TempDC.CreateCompatibleDC(&dc);

		CBitmap memBmp;
		memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		CBitmap* pOldBitmap = TempDC.SelectObject(&memBmp);

		TempDC.FillSolidRect(&rcClient, RGB(0, 0, 0));

		rcClientTemp.left++;
		rcClientTemp.top++;
		rcClientTemp.right--;
		rcClientTemp.bottom--;
		TempDC.FillSolidRect(&rcClientTemp, RGB(255, 255, 255));



		BLENDFUNCTION blend;
		memset( &blend, 0, sizeof( blend) );
		blend.BlendOp= AC_SRC_OVER;
		blend.SourceConstantAlpha = 128; // 透明度	
		//左
		dc.AlphaBlend(0, 0, 2, rcClient.Height(), &TempDC, 0, 0, 2, rcClient.Height(), blend);
		//上中
		dc.AlphaBlend(2, 0, rcClient.Width() - 2, 2, &TempDC, 2, 0, rcClient.Width() - 2, 2, blend);
		//右
		dc.AlphaBlend(rcClient.Width() - 2, 0, 2, rcClient.Height(), &TempDC, rcClient.Width() - 2, 0, 2, rcClient.Height(), blend);
		//下
		dc.AlphaBlend(2, rcClient.Height() - 2, rcClient.Width() - 2, 2, &TempDC, 2, rcClient.Height() - 2, rcClient.Width() - 2, 2, blend);

		for (int i = 0; i < 3; i++)
		{
			//左上
			dc.AlphaBlend(i, 3 - i, 1, 1, &TempDC, 0, 0, 1, 1, blend);
			dc.AlphaBlend(i + 1, 3 - i, 1, 1, &TempDC, 1, 1, 1, 1, blend);

			//右上
			dc.AlphaBlend(rcClient.right - 4 + i, i, 1, 1, &TempDC, 0, 0, 1, 1, blend);
			dc.AlphaBlend(rcClient.right - 4 + i, i + 1, 1, 1, &TempDC, 1, 1, 1, 1, blend);

			//左下
			dc.AlphaBlend(i, rcClient.bottom - 4 + i, 1, 1, &TempDC, 0, 0, 1, 1, blend);
			dc.AlphaBlend(i + 1, rcClient.bottom - 4 + i, 1, 1, &TempDC, 1, 1, 1, 1, blend);

			//右下
			dc.AlphaBlend(rcClient.right - 4 + i, rcClient.bottom - i - 1, 1, 1, &TempDC, 0, 0, 1, 1, blend);
			dc.AlphaBlend(rcClient.right - 4 + i, rcClient.bottom - i - 2, 1, 1, &TempDC, 1, 1, 1, 1, blend);

		}

		//// Title 标题
		//CString szTitile;
		//GetWindowText(szTitile);

		////CFont* pOldFont = MemDC.SelectObject(&m_TitleFont);

		//dc.SetBkMode(TRANSPARENT);
		//// 阴影
		//dc.SetTextColor(RGB(0, 0, 0));
		//CRect	rcText(8, 8, 100 , 20 );
		//dc.DrawText(szTitile, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		//// 文字
		//rcText.OffsetRect(-1, -1);
		//dc.SetTextColor(RGB(255, 255, 255));
		//dc.DrawText(szTitile, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		////dc.SelectObject(pOldFont);


		TempDC.SelectObject(pOldBitmap);
		TempDC.DeleteDC();

		MemDC.SelectObject(pOldBitmap);
		memBmp.DeleteObject();
	}
	


	MemDC.DeleteDC();

}

int CDlgBase::DrawVerticalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rc, int nBeginTransparent/* = 0*/, int nEndTransparent/* = 100*/)
{
	BOOL bIsDownTransition = TRUE;
	if (nEndTransparent <= nBeginTransparent)
	{
		bIsDownTransition = FALSE;
		int nTemp = nBeginTransparent;
		nBeginTransparent = nEndTransparent;
		nEndTransparent = nTemp;
	}

	BLENDFUNCTION blend;
	memset( &blend, 0, sizeof( blend) );
	blend.BlendOp= AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;

	int nStartPosition = rc.top;
	int nWidth = rc.right - rc.left;
	int nHeight =  rc.bottom - rc.top;

	int nMinTransition = 255 - 255.0 * nBeginTransparent / 100.0;
	int nMaxTransition = 255.0 * (100 - nEndTransparent) / 100.0;
	double dTransition = (nMinTransition - nMaxTransition) / (double)nHeight;
	if (bIsDownTransition)
	{
		for (int i = 0; i < nHeight; i++)
		{
			blend.SourceConstantAlpha = nMinTransition/* -  nMaxTransition*/ - (dTransition * i); 
			dcDes.AlphaBlend(rc.left, nStartPosition + i,nWidth , 1, 
				&dcSrc, rc.left, nStartPosition + i, nWidth, 1, blend); 
			//TRACE("%d\n", blend.SourceConstantAlpha);
		}
	}
	else
	{
		for (int i = 0; i < nHeight; i++)
		{
			blend.SourceConstantAlpha = nMaxTransition + (dTransition * i); 
			dcDes.AlphaBlend(rc.left, nStartPosition + i,nWidth , 1, 
				&dcSrc, rc.left, nStartPosition + i, nWidth, 1, blend); 
			//TRACE("\t\t\t\t%d\n", blend.SourceConstantAlpha);
		}
	}
	//TRACE("\n");
	return blend.SourceConstantAlpha;
}

void CDlgBase::DrawBKImageCross(CDC &dc, CDC &dcTemp)
{
	BLENDFUNCTION blend;
	memset( &blend, 0, sizeof( blend) );
	blend.BlendOp= AC_SRC_OVER;
	blend.SourceConstantAlpha = 255; 

	int nStartX = m_sizeCustomBKImage.cx - m_nOverRegio;
	int nStartY = m_sizeCustomBKImage.cy - m_nOverRegio;
	double dRadius = (double)sqrt((double)(m_nOverRegio * m_nOverRegio + m_nOverRegio * m_nOverRegio));
	double dRadiusTemp = 0.0;

	dc.FillSolidRect(nStartX, nStartY, m_nOverRegio, m_nOverRegio, m_clrCustomBK);
 	for (int i = 0; i < m_nOverRegio; i++)
 	{
 		for (int j = 0; j < m_nOverRegio; j++)
 		{
			double dRadiusTemp  = (double)sqrt((double)(i * i + j * j));
			if (dRadiusTemp > 99)
			{
				dRadiusTemp = 99;
			}
			blend.SourceConstantAlpha = 255 - (2.55 * ((dRadiusTemp / m_nOverRegio) * 100));
 			dc.AlphaBlend(nStartX + i, nStartY + j, 1, 1, &dcTemp, nStartX + i, nStartY + j, 1, 1, blend);
 		} 	
 	}
}

bool IsCross(CRect &rc1, CRect &r2)  
{  
	CRect rcTmp;  
	rcTmp.left = max(rc1.left, r2.left);  
	rcTmp.top = max(rc1.top, r2.top);  
	rcTmp.right = min(rc1.right, r2.right);  
	rcTmp.bottom = min(rc1.bottom, r2.bottom);  
	return (rcTmp.right >= rcTmp.left && rcTmp.bottom >= rcTmp.top);  
}  

void CDlgBase::DrawControl(CDC &dc, const CRect &rcClient)
{
	for (int i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		if (pControlBase)
		{
			pControlBase->Draw(dc, m_pt, m_rcUpdate, m_bIsLButtonDown);
		}
	}

	for (int i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			if (pControlBase->GetVisible() && IsCross(pControlBase->GetRect(), m_rcUpdate))
			{
				pControlBase->Draw(dc, m_pt, m_rcUpdate, m_bIsLButtonDown);
			}			
		}
	}	
}


bool CDlgBase::SetCustomStyle(bool bIsImageStyle, CString strCustomBKImage/* = ""*/)
{
	bool bOldIsDefaultStyle = m_bIsImageStyle;
	m_bIsImageStyle = bIsImageStyle;
	return bOldIsDefaultStyle;
}
void CDlgBase::OnNcPaint()
{
}

UINT CDlgBase::OnNcHitTest(CPoint point)
{
	CRect	rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.left, -rc.top);
	ScreenToClient(&point);
	int x = point.x;
	int y = point.y;

	if ( x < m_nFrameLeftRightSpace && y < m_nFrameTopBottomSpace)
	{
		return	HTTOPLEFT;
	}
	if ( x <= m_nFrameLeftRightSpace && y >= rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOMLEFT;
	}
	if ( x > rc.right - m_nFrameLeftRightSpace && y < m_nFrameTopBottomSpace)
	{
		return	HTTOPRIGHT;
	}
	if ( x >= rc.right - m_nFrameLeftRightSpace && y >= rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOMRIGHT;
	}

	if ( x < m_nFrameLeftRightSpace)
	{
		return	HTLEFT;
	}
	if ( x >= rc.right - m_nFrameLeftRightSpace)
	{
		return	HTRIGHT;
	}
	if ( y < m_nFrameTopBottomSpace)
	{
		return	HTTOP;
	}
	if ( y > rc.bottom - m_nFrameTopBottomSpace)
	{
		return	HTBOTTOM;
	}
// 	if ( y <= m_nFrameTopSpace)
// 	{
// 		return	HTCAPTION;
// 	}
	return	HTCLIENT;
	//	return CDialog::OnNcHitTest(point);
}

void CDlgBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (!IsIconic())
	{
		if (m_bIsImageStyle)
		{
			BOOL bIsMaximize = IsZoomed();
			int border_offset[] = {3, 2, 1};
			if (bIsMaximize)
			{				
				SetupRegion(border_offset, 0);
				m_nFrameLeftRightSpace = m_nFrameTopBottomSpace = 0;
			}
			else
			{
				SetupRegion(border_offset, 3);
				m_nFrameLeftRightSpace = m_nFrameTopBottomSpace = 3;
			}
			CControlBase *pControlBaseMax = GetControl(2);
			CControlBase *pControlBaseRestore = GetControl(3);
			if (pControlBaseMax && pControlBaseRestore)
			{
				pControlBaseMax->SetVisible(!bIsMaximize);
				pControlBaseRestore->SetVisible(bIsMaximize);
			}
		}
		else
		{
			int border_offset[] = {5, 3, 2, 1, 1};
			SetupRegion(border_offset, 5);
		}		
	}

	CRect rc;
	GetClientRect(&rc);
	OnSize(rc);
	InvalidateRect(NULL);
}

void CDlgBase::OnSize(CRect rcClient)
{
	for (int i = 0; i < m_vecArea.size(); i++)
	{
		CControlBase * pControlBase = m_vecArea.at(i);
		CRect rcTemp;
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();
			
			if (0 == uControlID)
			{
				rcTemp.SetRect(0, rcClient.bottom - 33, rcClient.right, rcClient.bottom);
			}
			else if (-1 == uControlID)
			{
				rcTemp.SetRect(0, 88, rcClient.right, rcClient.bottom);
			}
			else if (-2 == uControlID)
			{
				rcTemp.SetRect(0, 44, rcClient.right, 88);
			}
			else
			{
				continue;
			}
			pControlBase->SetRect(rcTemp);
		}
	}
	for (int i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		CRect rcTemp;
		if (pControlBase)
		{
			UINT uControlID = pControlBase->GetControlID();
			if (0 == uControlID)
			{
				rcTemp.SetRect(0, rcClient.bottom - 33, rcClient.right, rcClient.bottom);
			}
			else if (1 == uControlID)
			{
				int nStartButton = rcClient.Width() - 39;
				rcTemp.SetRect(nStartButton, -2, nStartButton + 39, 18);
				
			}
			else if (2 == uControlID)
			{
				int nStartButton = rcClient.Width() - 39;
				nStartButton -= 28 ;
				rcTemp.SetRect(nStartButton, -2, nStartButton + 28, 18);
			}
			else if (3 == uControlID)
			{
				int nStartButton = rcClient.Width() - 39;
				nStartButton -= 28;
				rcTemp.SetRect(nStartButton, -2, nStartButton + 28, 18);
			}
			else if(4 == uControlID)
			{
				int nStartButton = rcClient.Width() - 39;
				nStartButton -= 28 * 2;
				rcTemp.SetRect(nStartButton, -2, nStartButton + 28, 18);
			}
			else if (5 == uControlID)
			{
				int nStartButton = rcClient.right - 100;
				rcTemp.SetRect(nStartButton, rcClient.bottom - 29,nStartButton + 86, rcClient.bottom - 5);
			}
			else if (6 == uControlID)
			{
				int nStartButton = rcClient.right - 200;
				rcTemp.SetRect(nStartButton, rcClient.bottom - 29,nStartButton + 86, rcClient.bottom - 5);
			}
			else
			{
				continue;
			}
			pControlBase->SetRect(rcTemp);
		}
	}
}

// 设置窗口区域
void CDlgBase::SetupRegion(int border_offset[], int nSize)
{
	CDC* pDC = GetDC();

	CRect	rc;
	GetWindowRect(rc);
	rc.OffsetRect(-rc.left, -rc.top);

	CRgn	rgn;
	rgn.CreateRectRgn(0, 0, rc.Width(), rc.Height());
	CRgn	rgn_xor;
	CRect	rcXor;
	
	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(0, y, border_offset[y], y + 1);
		rgn_xor.CreateRectRgn(0, y, border_offset[y], y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	for (int y = 0; y < nSize; ++y)
	{
		rcXor.SetRect(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn_xor.CreateRectRgn(rc.right - border_offset[y], y, rc.right, y + 1);
		rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
		rgn_xor.DeleteObject();
	}

	if(m_bIsImageStyle)
	{
		for (int y = 0; y < nSize; ++y)
		{
			rcXor.SetRect(0, rc.bottom - y - 1, border_offset[y],  rc.bottom - y);
			rgn_xor.CreateRectRgn(0, rc.bottom - y - 1, border_offset[y],  rc.bottom - y);
			rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
			rgn_xor.DeleteObject();
		}

		for (int y = 0; y < nSize; ++y)
		{
			rcXor.SetRect(rc.right - border_offset[y], rc.bottom - y - 1, rc.right, rc.bottom -  y);
			rgn_xor.CreateRectRgn(rc.right - border_offset[y], rc.bottom - y - 1, rc.right,rc.bottom -  y);
			rgn.CombineRgn(&rgn, &rgn_xor, RGN_XOR);
			rgn_xor.DeleteObject();
		}
  		HWND hWnd = GetSafeHwnd();
  		SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE) | WS_EX_LAYERED);
  		SetLayeredWindowAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY );		
	}
	SetWindowRgn((HRGN)rgn, TRUE);
	m_Rgn.DeleteObject();
	m_Rgn.Attach(rgn.Detach());
	ReleaseDC(pDC);
}

void CDlgBase::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);

	CRect	rcWindow;

	if (bCalcValidRects && lpncsp->lppos)
	{
		rcWindow.SetRect(lpncsp->lppos->x, lpncsp->lppos->y,
			lpncsp->lppos->x + lpncsp->lppos->cx,
			lpncsp->lppos->y + lpncsp->lppos->cy);
	}
	else
	{
		GetWindowRect(rcWindow);
	}

	lpncsp->rgrc[0] = rcWindow;
 
}

void CDlgBase::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (lpwndpos->cx < m_MinSize.cx)
	{
		lpwndpos->cx = m_MinSize.cx;
	}
	if (lpwndpos->cy < m_MinSize.cy)
	{
		lpwndpos->cy = m_MinSize.cy;
	}
}

void CDlgBase::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	//	CDialog::OnWindowPosChanged(lpwndpos);

	::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
		| SWP_NOSENDCHANGING | SWP_NOACTIVATE);

	SendMessage(WM_MOVE, 0, MAKELPARAM(lpwndpos->x, lpwndpos->y));
	SendMessage(WM_SIZE, 0, MAKELPARAM(lpwndpos->cx, lpwndpos->cy));
}

void CDlgBase::PostNcDestroy()
{

}

void CDlgBase::OnClose()
{
	OnCancel();
}

void CDlgBase::OnMinimize()
{
	ShowWindow(SW_MINIMIZE);
}
BOOL CDlgBase::OnMaximize()
{

 	if (IsZoomed())
 	{
 		ShowWindow(SW_RESTORE);
		return FALSE;
 	}
 	else
 	{
 		//TRACE("\n\n");
 		ShowWindow(SW_SHOWMAXIMIZED);
		return TRUE;
 	}
}



LRESULT CDlgBase::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = DefWindowProc(WM_SETTEXT, wParam, lParam);
	return	hr;
}


BOOL CDlgBase::OnNcActivate(BOOL bActive)
{
	m_bNCActive = (bool)bActive;

	return TRUE;	
}


void CDlgBase::PreSubclassWindow()
{


	CDialog::PreSubclassWindow();
}


void CDlgBase::OnMouseMove(UINT nFlags, CPoint point) 
{	
	//TRACE("客户区鼠标移动 X = %d, Y = %d\n", m_pt.x, m_pt.y);
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;//HOVER_DEFAULT;
		m_bTracking = ::_TrackMouseEvent(&tme);
	}

	if (m_pControl)
	{
		CRect rc = m_pControl->GetRect();
		if(rc.PtInRect(point) && m_bTracking)
		{
			//TRACE("还在 %d\n", m_pControl->GetControlID());
			m_pt = point;
			if(m_pControl->IsDraw(m_pt))
			{
				InvalidateRect(&m_pControl->GetRect());
			}
			return;
		}	
	}

	if (!m_bIsLButtonDown)
	{
		m_pt = point;

		CControlBase * pOldControl = m_pControl;
		BOOL bIsSelect = FALSE;

		if(m_bTracking)
		{
			for (int i = 0; i < m_vecControl.size(); i++)
			{
				CControlBase * pControlBase = m_vecControl.at(i);
				if (pControlBase)
				{
					CRect rc = pControlBase->GetRect();
					if(rc.PtInRect(point) && pControlBase->GetVisible())
					{
						m_pControl = pControlBase;
						//TRACE("选中 %d\n", m_pControl->GetControlID());
						bIsSelect = TRUE;
						break;
					}
				}		
			}
		}

		if (pOldControl)
		{
			TRACE("擦除 %d\n", pOldControl->GetControlID());
			if (!pOldControl->GetTiemrIsRun())
			{
				InvalidateRect(&pOldControl->GetRect());
			}
			
			//pOldControl->KillTimer();
			//TRACE("擦除 %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
		}

		if (bIsSelect)
		{
			if (m_pControl)
			{
				//TRACE("第一次绘 %d\n", m_pControl->GetControlID());
				InvalidateRect(&m_pControl->GetRect());
				if(!m_pControl->GetTiemrIsRun())
				{
					m_pControl->SetTimer(150);
				}
				
				//TRACE("第一次绘 %d, %d, %d, %d\n", rc.left, rc.top, rc.right, rc.bottom);
			}
		}
		else
		{
			//TRACE("客户区 m_pControl = NULL X = %d, Y = %d\n", point.x, point.y);
			m_pControl = NULL;
		}
	}
	
	//CDialog::OnMouseMove(nFlags, point);
}

LRESULT CDlgBase::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	//TRACE("鼠标离开客户区\n");
	if (!m_bIsLButtonDown)
	{
		if (m_pControl)
		{
			//TRACE("鼠标离开客户区 重绘 %d\n", m_pControl->GetControlID());
			CRect rc = m_pControl->GetRect();
			if (IsZoomed())
			{
				CPoint point;
				GetCursorPos(&point);
				ScreenToClient(&point);

				if(rc.PtInRect(point))
				{
					m_pt = point;
				}
				else
				{
					m_pt.SetPoint(-1, -1);
				}
			}
			else
			{
				m_pt.SetPoint(-1, -1);
			}
			
			InvalidateRect(&rc);
		}
		//TRACE("离开客户区 m_pControl = NULL\n");
		m_pControl = NULL;
	}
	
	return 0;
}

LRESULT CDlgBase::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	//TRACE("鼠标进入客户区\n");
 	if (m_pControl)
 	{
 		CPoint point;
 		GetCursorPos(&point);
 		ScreenToClient(&point);
 		CRect rc = m_pControl->GetRect();
 		if (rc.PtInRect(point))
 		{
 			//TRACE("鼠标进入客户区 %d\n", m_pControl->GetControlID());
 			m_pt = point;
 			InvalidateRect(&rc);			
 		}
 	}
	return 0;
}

void CDlgBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	//TRACE("\n\n\n");
	//TRACE("客户区左键按下\n");

	BOOL bIsSelect = false;

	if (m_pControl)
	{
		//TRACE("客户区左键按下 还有\n");
		CRect rc = m_pControl->GetRect();
		if (rc.PtInRect(point))
		{
			//TRACE("按下 %d\n", m_pControl->GetControlID());
			bIsSelect = TRUE;
			m_bIsLButtonDown = TRUE;
			OnControlLButtonDown(m_pControl);
			//TRACE("左键按下 设置区域\n");
			m_rcLButtonDown = rc;
			InvalidateRect(&rc);			
		}
	}
	
	if (bIsSelect && !m_bIsSetCapture)
	{
		//TRACE("客户区捕捉鼠标\n");
		SetCapture();//客户区捕捉鼠标
		m_bIsSetCapture = TRUE;
		
		return;
	}

	//TRACE("客户区左键按下 清空 m_rcLButtonDown\n");
	m_rcLButtonDown.SetRect(-1, -1, -1, -1);
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	//TRACE("客户区左键释放\n");
	if (m_bIsSetCapture)
	{
		//TRACE("客户区释放鼠标\n");
		ReleaseCapture();//客户区释放鼠标 
		m_bIsSetCapture = false;
	}

	m_bIsLButtonDown = FALSE;

	if (m_pControl)
	{
		//TRACE("释放 %d\n", m_pControl->GetControlID());		
		CRect rc = m_pControl->GetRect();
		
		if (m_rcLButtonDown.PtInRect(point))
		{
			//TRACE("执行 %d\n", m_pControl->GetControlID());
			m_pt = point;
			OnControlLButtonUp(m_pControl);
		}
		else
		{
			//TRACE("已经不在 %d\n", m_pControl->GetControlID());
			if (rc.PtInRect(point))
			{
				m_pt = point;
			}
			else
			{
				m_pt.SetPoint(-1, -1);
				m_pControl = NULL;
			}
		}	
		//TRACE("左键释放 m_pControl = NULL\n");
		InvalidateRect(&rc);
	}
	
	m_bIsLButtonDblClk = FALSE;
	//TRACE("客户区左键释放 清空 m_rcLButtonDown\n");
	m_rcLButtonDown.SetRect(-1, -1, -1, -1);
	TRACE("\n\n\n");
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgBase::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//TRACE("客户区左键双击\n");

	for (int i = 0; i < m_vecControl.size(); i++)
	{
		CControlBase * pControlBase = m_vecControl.at(i);
		if (pControlBase)
		{
			CRect rc = pControlBase->GetRect();
			if(rc.PtInRect(point))
			{
				//TRACE("左键双击 设置区域\n");
				m_rcLButtonDown = rc;
				//OnControlLButtonDblClk(m_pControl);				
				m_bIsLButtonDblClk = TRUE;
				return;;
			}
		}		
	}

	OnMaximize();

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDlgBase::OnControlLButtonDown(CControlBase * pControl)
{

}


void CDlgBase::OnControlLButtonUp(CControlBase * pControl)
{
	if (pControl)
	{
		if (pControl->GetDisable() || !pControl->GetVisible())
		{
			return;
		}
		UINT uControlID = pControl->GetControlID();

		//TRACE("执行 %d\n", uControlID);

		if (1 == uControlID)
		{
			OnClose();
		}
		else if (2 == uControlID || 3 == uControlID)
		{
			OnMaximize();
		}
		else if (4 == uControlID)
		{
			OnMinimize();
		}
	}
}


void CDlgBase::OnControlLButtonDblClk(CControlBase * pControl)
{
	if (pControl)
	{
		UINT uControlID = pControl->GetControlID();
		//TRACE("双击 %d\n", uControlID);
	}
}
BOOL CDlgBase::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE ) 
		{
			//return TRUE;
		}
		if( pMsg->wParam == VK_RETURN  )
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
