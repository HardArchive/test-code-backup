	const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreate)

/******************************************************************************* 
1、 函数名称： virtual LRESULT OnTaskBarCreate(WPARAM wParam, LPARAM lParam)
2、 功能描述： 重建任务栏响应消息函数
3、 输入参数： 
4、 返 回 值： 
5、 动态内存： 
6、 代码设计：  阿毛
7、 开发日期： 2011-10-12 20:21:58
8、 备    注： 
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
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_TASKBARCREATED == message)
	{
		InitTray();
	}

	return CDialog::WindowProc(message, wParam, lParam);
}