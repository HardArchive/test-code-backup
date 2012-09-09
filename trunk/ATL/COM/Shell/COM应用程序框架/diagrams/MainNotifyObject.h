/********************************************************************
	created:	2006/06/16
	created:	16:6:2006   17:53
	filename: 	f:\Test\ComFrame\ArtFrame\Test_ArtFrame\MainNotifyObject.h
	file path:	f:\Test\ComFrame\ArtFrame\Test_ArtFrame
	file base:	MainNotifyObject
	file ext:	h
	author:		YTF-1978
	
	purpose:	定义MDI主窗口命令响应类，用来响应全局通一的命令处理。
	
	Class Name: CMainNotifyObject.

*********************************************************************/
#pragma once

#include "..\inc\ArtFrameImpl.h"
#include "resource.h"

class CMainNotifyObject : public CNotifyObjectImpl
{
public:
	CMainNotifyObject(void);
	~CMainNotifyObject(void);

	IMDIFrame *m_lpMdiFrame;

	void WinMain();

	NC_BEGIN_MAP()
		//NC_COMMAND_ID_HANDLER(ID_FILE_NEW,FileNew)
		//NC_COMMAND_ID_HANDLER(ID_FILE_EXIT,OnFileExit)
		//NC_COMMAND_ID_HANDLER(ID_HELP_ABOUT,OnAbout)
	NC_END_MAP()

	HRESULT CreateMain();
	void FileNew(LPNotifyCmdStruct lpnc);
	void OnFileExit(LPNotifyCmdStruct lpnc);
	void OnAbout(LPNotifyCmdStruct lpnc);
	
};
