#pragma once


// TextView view

class CTextView : public CEditView
				,public CDataViewImpl<CTextView>
{
	DECLARE_DYNCREATE(CTextView)
	CEdit *m_pEdit;
	CFont font;
protected:
public:
	CTextView();           // protected constructor used by dynamic creation
	virtual ~CTextView();
	NC_BEGIN_MAP()
		NC_COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCopy)		
		NC_COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnPaste)
		NC_COMMAND_ID_HANDLER(ID_EDIT_SELECT_ALL, OnSelectAll)
		NC_COMMAND_ID_HANDLER(ID_EDIT_CLEAR, OnClear)
		NC_COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnUndo)
		//NC_COMMAND_ID_HANDLER(ID_EDIT_CUT, OnCut) //这两个命令处理使用标准windows WM_COMMAND 消息处理。
		//NC_COMMAND_ID_HANDLER(ID_FORMAT_FONT, OnFormatFont)
		NC_COMMAND_TO_WND(m_hWnd)//把NotifyCommand 发送给当m_hWnd窗口。使用标准windows WM_COMMAND 消息处理。
	NC_END_MAP()
	void OnCopy(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnPaste(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnSelectAll(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnClear(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnUndo(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	//void OnFormatFont(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	//void OnCut(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);

	virtual HRESULT STDMETHODCALLTYPE CreateWnd(HWND hWndParent);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditCut();
	afx_msg void OnFormatFont();
};


