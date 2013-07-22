class CTestpipeDlg : public CDialog
{
public:
	CTestpipeDlg(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_TESTPIPE_DIALOG };
	CString	m_sServer;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGettime();

	DECLARE_MESSAGE_MAP()
};

