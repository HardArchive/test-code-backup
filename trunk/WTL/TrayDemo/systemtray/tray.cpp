	const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreate)

/******************************************************************************* 
1�� �������ƣ� virtual LRESULT OnTaskBarCreate(WPARAM wParam, LPARAM lParam)
2�� ���������� �ؽ���������Ӧ��Ϣ����
3�� ��������� 
4�� �� �� ֵ�� 
5�� ��̬�ڴ棺 
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-10-12 20:21:58
8�� ��    ע�� 
********************************************************************************/ 
LRESULT CReplacePictureDlg::OnTaskBarCreate(WPARAM wParam, LPARAM lParam)
{
	// Reinstall taskbar icon
	//HICON hIcon = m_nid.hIcon;
	//m_nid.hIcon = NULL;
	//if (hIcon)
	//	SetIcon(hIcon, NULL); // will reuse current tip
	return 0;
}


LRESULT CReplacePictureDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_TASKBARCREATED == message)
	{
		InitTray();
	}

	return CDialog::WindowProc(message, wParam, lParam);
}