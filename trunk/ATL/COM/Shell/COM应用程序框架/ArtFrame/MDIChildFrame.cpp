// MDIChildFrame.cpp : Implementation of CMDIChildFrame

#include "stdafx.h"
#include "MDIChildFrame.h"
#include ".\mdichildframe.h"


// CMDIChildFrame
LRESULT CMDIChildFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPMSG pMsg = (LPMSG)lParam;

	if(CMDIChildWindowImpl<CMDIChildFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return FALSE;// m_view.PreTranslateMessage(pMsg);
}



STDMETHODIMP CMDIChildFrame::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMDIChildFrame
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}

	return S_FALSE;
}


//���һ����ͼ��
STDMETHODIMP CMDIChildFrame::PushDataView(IDataView* lpDataView)
{
	if (lpDataView!= NULL)
	{
		//m_coll.push_back(lpDataView);
		lpDataView->CreateWnd(m_hWnd);
		put_ActiveChildWindow(lpDataView);

		return S_OK;
	}

	return S_FALSE;
}



STDMETHODIMP CMDIChildFrame::PopDataView(IDataView *lpCW)
{
	//�Ƴ�MDI����
	//...

	return S_OK;
}



STDMETHODIMP CMDIChildFrame::get_ActiveChildWindow(IDataView** pVal)
{
	m_pActiveDataView->AddRef();
	*pVal = m_pActiveDataView;
	
	return S_OK;
}


STDMETHODIMP CMDIChildFrame::put_ActiveChildWindow(IDataView* newVal)
{
	m_pActiveDataView = newVal;
	ATLASSERT(NULL != m_pActiveDataView);
	CRect rect;

	if (NULL != m_pActiveDataView)
	{
		//���һ��������ͼ
		m_pActiveDataView->GetWnd(&m_hWndClient);
		GetWindowRect(&rect);
		::MoveWindow(m_hWndClient, 0, 0, rect.right, rect.bottom, TRUE);
		::ShowWindow(m_hWndClient, SW_SHOW);
		return S_OK;
	}

	//��Ӵ�����
	//...

	return S_FALSE;
}


LRESULT CMDIChildFrame::OnMDISetMenu(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HMENU hFrameMenu = (HMENU)wParam;
	HMENU hWndMenu = (HMENU)lParam;
	SetMsgHandled(TRUE);

	return 0;
}

LRESULT CMDIChildFrame::MDIActivate(HWND hwndDeactivate, HWND hwndActivate)
{
	g_MDIFrame->m_pActiveChildFrame = this;

	return 0;
}

LRESULT CMDIChildFrame::OnActivate(UINT state, BOOL fMinimized, HWND hwndActDeact)
{

	return 0;
}


STDMETHODIMP CMDIChildFrame::get_Title(BSTR* pVal)
{
	GetWindowText(*pVal);

	return S_OK;
}

STDMETHODIMP CMDIChildFrame::put_Title(BSTR newVal)
{
	SetWindowText(newVal);

	return S_OK;
}

LRESULT CMDIChildFrame::OnCommand(UINT codeNotify, int id, HWND hwndCtl)
{
	VARIANT_BOOL bHandle = VARIANT_FALSE;

	
	if (m_vec.GetSize() > 0)
	{
		//ִ�д��ڵ��¼�
		Fire_NotifyCommand(codeNotify, id, &bHandle);
		if (bHandle == VARIANT_FALSE)
		{
			return 0;
		}
	}


	//װ�����͸�"������ͼ"
	if (NULL != m_pActiveDataView)
	{
		m_pActiveDataView->NotifyCommand(codeNotify, id, &bHandle);
	}


	return 0;
}

STDMETHODIMP CMDIChildFrame::get_CommandBars(IDispatch** pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}
