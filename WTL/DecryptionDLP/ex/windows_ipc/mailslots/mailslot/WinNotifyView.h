class CWinNotifyView : public CFormView
{
protected:
	CWinNotifyView();
	DECLARE_DYNCREATE(CWinNotifyView)

	enum { IDD = IDD_MAINFRAME };
	CString	m_sMessage;
	CString	m_sFrom;
	CString	m_sTimeAndTo;

protected:
	virtual ~CWinNotifyView();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnUpdateMessage(CCmdUI* pCmdUI);
  
	DECLARE_MESSAGE_MAP()

  CWinNotifyDoc* GetDoc() const;
  UINT_PTR m_nTimerID;
};


