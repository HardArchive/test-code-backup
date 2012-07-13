#pragma once

class CControlBase
{
public:
	CControlBase(HWND hWnd, UINT uControlID, CRect rc, CString strTitle = "", BOOL bIsVisible = TRUE, BOOL bIsDisable = FALSE ,
		int nBeginTransparent = 100,int nEndTransparent = 100 ,BOOL bIsPressDown = FALSE) ;
	virtual ~CControlBase(void);

	virtual void Draw(CDC &dc, const CPoint point, CRect rcUpdate, BOOL bIsPressDown) = 0;
	virtual void OnTimer() = 0;
	virtual BOOL IsDraw(CPoint point) { return FALSE; };
	int DrawVerticalTransition(CDC &dcDes, CDC &dcSrc, const CRect &rc, int nBeginTransparent = 0, int nEndTransparent = 100);
	int DrawTransitionSpace(CDC &dcDes, const CRect &rc, int nBeginTransparent = 40, int nEndTransparent = 90);
	void SetRect(CRect rc) { m_rc = rc;};
	CRect GetRect() { return m_rc;};
	UINT GetControlID() {return m_uID;};
	void SetVisible(BOOL bIsVisible) { m_bIsVisible = bIsVisible;};
	BOOL GetVisible() {return m_bIsVisible;};
	void SetDisable(BOOL bIsDisable) { m_bIsDisable = bIsDisable;};
	BOOL GetDisable() {return m_bIsDisable;};

	BOOL GetTiemrIsRun();
	BOOL SetTiemrIsRun(BOOL bIsTimer);
	int SetTimer(int nTimerResolution, int nRunNum = 0);
	int KillTimer();
	static void CALLBACK TimerCallProc(UINT TimerID2, UINT msg, DWORD dwUser, DWORD dwa, DWORD dwb);;
protected:
	UINT	m_uID;					// 控件 ID
	CRect	m_rc;					// 区域
	BOOL	m_bIsVisible;			// 可见
	BOOL	m_bIsDisable;			// 是否可用
	int		m_nBeginTransparent;	// 背景 透明度 开始
	int		m_nEndTransparent;		// 背景 透明度 结束
	BOOL	m_bIsSelect;			// 是否选中
	BOOL	m_bIsPressDown;			// 是否按下
	CString	m_strTitle;				// 标题
	HWND	m_hWnd;
	CFont	m_font;

	//定显示器相关
	int m_nTimerResolution;		//定时器间隔
	int m_TimerID;				//定时器ID
	UINT m_wAccuracy;			//定义分辨率 
	bool m_bIsTmierOK;			//是否可以定时 
	BOOL m_bIsTimer;			//是否有定时功能
};
