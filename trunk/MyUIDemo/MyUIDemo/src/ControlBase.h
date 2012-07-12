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
	UINT	m_uID;					// �ؼ� ID
	CRect	m_rc;					// ����
	BOOL	m_bIsVisible;			// �ɼ�
	BOOL	m_bIsDisable;			// �Ƿ����
	int		m_nBeginTransparent;	// ���� ͸���� ��ʼ
	int		m_nEndTransparent;		// ���� ͸���� ����
	BOOL	m_bIsSelect;			// �Ƿ�ѡ��
	BOOL	m_bIsPressDown;			// �Ƿ���
	CString	m_strTitle;				// ����
	HWND	m_hWnd;
	CFont	m_font;

	//����ʾ�����
	int m_nTimerResolution;		//��ʱ�����
	int m_TimerID;				//��ʱ��ID
	UINT m_wAccuracy;			//����ֱ��� 
	bool m_bIsTmierOK;			//�Ƿ���Զ�ʱ 
	BOOL m_bIsTimer;			//�Ƿ��ж�ʱ����
};
