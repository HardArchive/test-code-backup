/********************************************************************
 *
 *	created:	2006/06/15
 *	created:	15:6:2006   14:36
 *	filename: 	f:\Test\ComFrame\Temp_WTLMDI\TextView.h
 *	file path:	f:\Test\ComFrame\Temp_WTLMDI
 *	file base:	TextView
 *	file ext:	h
 *	author:		许宗森
 *	Eamil:		ytf1978@163.com
 *	purpose:	文本视图。
 *
 *				用来测试 COM MDI应用程序框架类。
 *
 *				CTextView.
 *
 ********************************************************************/

#pragma once

#include "..\inc\ArtFrameImpl.h"
#include <atlcrack.h>

// TextView
class CTextView :public CWindowImpl<CTextView, CEdit>
				,public CDataViewImpl<CTextView>
{
private:
	CFont font;
public:
	CTextView();
	~CTextView();

	NC_BEGIN_MAP()
		NC_COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCopy)
		NC_COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnPaste)
		NC_COMMAND_ID_HANDLER(ID_EDIT_SELECT_ALL, OnSelectAll)
		NC_COMMAND_ID_HANDLER(ID_EDIT_CLEAR, OnClear)
		NC_COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnUndo)
		//NC_COMMAND_ID_HANDLER(ID_FORMAT_FONT, OnFormatFont)  //这两个命令处理使用标准windows WM_COMMAND 消息处理。
		//NC_COMMAND_ID_HANDLER(ID_EDIT_CUT, OnCut)
		NC_COMMAND_TO_WND(m_hWnd);	//把NotifyCommand 发送给当m_hWnd窗口。使用标准windows WM_COMMAND 消息处理。
	NC_END_MAP()

	void OnCopy(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnPaste(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnSelectAll(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnClear(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnUndo(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	//void OnCut(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	//void OnFormatFont(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);

	virtual HRESULT STDMETHODCALLTYPE CreateWnd(HWND hWndParent);

	DECLARE_WND_SUPERCLASS(L"TextView", CEdit::GetWndClassName())

    BEGIN_MSG_MAP(CTextView)
		//使用标准windows WM_COMMAND 消息处理。
		COMMAND_ID_HANDLER_EX(ID_FORMAT_FONT, OnFormatFont)
		COMMAND_ID_HANDLER_EX(ID_EDIT_CUT, OnEditCut)
    END_MSG_MAP()


	LRESULT OnFormatFont(WORD wNotifyCode, WORD wID, HWND hWndCtl);
	LRESULT OnEditCut(WORD wNotifyCode, WORD wID, HWND hWndCtl);
};


