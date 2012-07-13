

#ifndef __DLG_MY_BASE_X_H__
#define __DLG_MY_BASE_X_H__
#include <vector>
#include "ControlBase.h"


using namespace  std;
// 可拉抻对话框
class  CDlgBase : public CDialog
{
	DECLARE_DYNAMIC(CDlgBase)

public:

	CDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CDlgBase();

public:

	void SetMinSize(int iWidth, int iHeight);	// 设置最小窗口大小
	SIZE GetMinSize();
	void GetFrameRect(RECT* pRect);				// 取得窗口框架客户区
	void SetIcon(HICON hIcon, BOOL bBigIcon);
	bool SetIcon(int IID, BOOL bBigIcon);
	bool IsFocus();

protected:

	CSize			m_MinSize;		// 窗口限定最小大小
	CRect			m_FrameRect;	// 窗口可绘制区域
	CRgn			m_Rgn;			// 不规则窗口区域
	COLORREF		m_clrCustomBK;	// 自定义前景颜色

	CFont			m_TitleFont;	// 绘制标题栏的字体
	HICON			m_hIcon;		// 窗口图标 16 X 16

	CBitmap			m_defaultBKImage;	// 框架背景图片
	CBitmap			m_CustomBKImage;	// 框架背景图片
	CSize			m_sizeCustomBKImage;
	CDC				m_MemBKDC;			//背景dc
	CBitmap			*m_pOldMemBK;
	CBitmap			m_MemBK;


	int				m_nFrameTopBottomSpace;
	int				m_nFrameLeftRightSpace;

	int				m_nOverRegio;		//过度的大小
	bool			m_bNCActive;
	bool			m_bIsImageStyle;

	bool			m_bTracking;
	bool			m_bIsLButtonDown;
	CRect			m_rcLButtonDown;
	bool			m_bIsLButtonDblClk;
	bool			m_bIsSetCapture;

	CPoint			m_pt;
	CRect			m_rcUpdate;
	vector<CControlBase *>	m_vecControl;
	vector<CControlBase *>	m_vecArea;
	CControlBase * m_pControl;


	CString m_strPath;
protected:

	// 设置不规则窗体区域
	void SetupRegion(int border_offset[], int nSize);
	void DrawImageStyle(CDC &dc, const CRect &rcClient);
	void DrawBKImage(BOOL bIsCustomBKImage);					//画背景图片
	virtual void DrawControl(CDC &dc, const CRect &rcClient);	//前景图片
	void DrawBKImageCross(CDC &dc, CDC &dcTemp);
	
	int DrawVerticalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rc, int nBeginTransparent = 0, int nEndTransparent = 100);

	void InitUI();
	CControlBase *GetControl(UINT uControlID);

	void OnControlLButtonDown(CControlBase * pControl);
	void OnControlLButtonUp(CControlBase * pControl);
	void OnControlLButtonDblClk(CControlBase * m_pControl);

	// 重载绘制窗体内容	
	virtual void OnSize(CRect rcFrame);						// 窗口大小改变时调用 // 重载实现动态窗口大小
	virtual void OnClose();									// 处理 WM_CLOSE 决定是否关闭窗口
	virtual void OnMinimize();
	virtual BOOL OnMaximize();

	//{{AFX_MSG{CDlgBase}
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd (CDC* pDC);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);


	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	bool SetCustomStyle(bool bIsImageStyle, CString strCustomBKImage = "");
protected:
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif __DLG_MY_BASE_X_H__

