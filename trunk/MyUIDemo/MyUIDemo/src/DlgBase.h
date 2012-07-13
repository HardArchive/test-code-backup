

#ifndef __DLG_MY_BASE_X_H__
#define __DLG_MY_BASE_X_H__
#include <vector>
#include "ControlBase.h"


using namespace  std;
// �����ӶԻ���
class  CDlgBase : public CDialog
{
	DECLARE_DYNAMIC(CDlgBase)

public:

	CDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CDlgBase();

public:

	void SetMinSize(int iWidth, int iHeight);	// ������С���ڴ�С
	SIZE GetMinSize();
	void GetFrameRect(RECT* pRect);				// ȡ�ô��ڿ�ܿͻ���
	void SetIcon(HICON hIcon, BOOL bBigIcon);
	bool SetIcon(int IID, BOOL bBigIcon);
	bool IsFocus();

protected:

	CSize			m_MinSize;		// �����޶���С��С
	CRect			m_FrameRect;	// ���ڿɻ�������
	CRgn			m_Rgn;			// �����򴰿�����
	COLORREF		m_clrCustomBK;	// �Զ���ǰ����ɫ

	CFont			m_TitleFont;	// ���Ʊ�����������
	HICON			m_hIcon;		// ����ͼ�� 16 X 16

	CBitmap			m_defaultBKImage;	// ��ܱ���ͼƬ
	CBitmap			m_CustomBKImage;	// ��ܱ���ͼƬ
	CSize			m_sizeCustomBKImage;
	CDC				m_MemBKDC;			//����dc
	CBitmap			*m_pOldMemBK;
	CBitmap			m_MemBK;


	int				m_nFrameTopBottomSpace;
	int				m_nFrameLeftRightSpace;

	int				m_nOverRegio;		//���ȵĴ�С
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

	// ���ò�����������
	void SetupRegion(int border_offset[], int nSize);
	void DrawImageStyle(CDC &dc, const CRect &rcClient);
	void DrawBKImage(BOOL bIsCustomBKImage);					//������ͼƬ
	virtual void DrawControl(CDC &dc, const CRect &rcClient);	//ǰ��ͼƬ
	void DrawBKImageCross(CDC &dc, CDC &dcTemp);
	
	int DrawVerticalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rc, int nBeginTransparent = 0, int nEndTransparent = 100);

	void InitUI();
	CControlBase *GetControl(UINT uControlID);

	void OnControlLButtonDown(CControlBase * pControl);
	void OnControlLButtonUp(CControlBase * pControl);
	void OnControlLButtonDblClk(CControlBase * m_pControl);

	// ���ػ��ƴ�������	
	virtual void OnSize(CRect rcFrame);						// ���ڴ�С�ı�ʱ���� // ����ʵ�ֶ�̬���ڴ�С
	virtual void OnClose();									// ���� WM_CLOSE �����Ƿ�رմ���
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

