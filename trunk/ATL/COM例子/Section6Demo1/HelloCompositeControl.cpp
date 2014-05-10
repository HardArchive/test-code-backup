// HelloCompositeControl.cpp : CHelloCompositeControl 的实现
#include "stdafx.h"
#include "HelloCompositeControl.h"


// CHelloCompositeControl

LRESULT CHelloCompositeControl::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MessageBox(_T("我是Button1"));

	if (++m_lBtn1ClickCount%3 == 0)
	{
		//向控件调用者发送事件
		Fire_OnClickBtn(0, m_lBtn1ClickCount);
	}

	return 0;
}

LRESULT CHelloCompositeControl::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MessageBox(_T("我是Button2"));

	if (++m_lBtn2ClickCount%3 == 0)
	{
		//向控件调用者发送事件
		Fire_OnClickBtn(1, m_lBtn2ClickCount);
	}

	return 0;
}

STDMETHODIMP CHelloCompositeControl::get_Description(BSTR* pVal)
{
	//释放旧的字符串
	SysFreeString(*pVal);

	*pVal = m_bstrDescription.Copy();
	return S_OK;
}

STDMETHODIMP CHelloCompositeControl::put_Description(BSTR newVal)
{
	m_bstrDescription.AssignBSTR(newVal);
	return S_OK;
}

STDMETHODIMP CHelloCompositeControl::EnableButton(LONG lBtnIndex, VARIANT_BOOL varbEnable)
{
	if (lBtnIndex == 0)
	{
		//获取Button1的窗口句柄
		HWND hBtn = ::GetDlgItem(m_hWnd, IDC_BUTTON1);

		if (varbEnable)
		{
			//启用Button1
			::EnableWindow(hBtn, TRUE);		
		}
		else
		{
			//禁用Button
			::EnableWindow(hBtn, FALSE);
		}

		//或写右边这句代替上面几句  ::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON1), varbEnable? TRUE:FALSE);
	}
	else if (lBtnIndex == 1)
	{
		//获取Button2的窗口句柄
		HWND hBtn = ::GetDlgItem(m_hWnd, IDC_BUTTON2);

		if (varbEnable)
		{
			//启用Button2
			::EnableWindow(hBtn, TRUE);
		}
		else
		{
			//禁用Button2
			::EnableWindow(hBtn, FALSE);
		}

		//或写右边这句代替上面几句  ::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON2), varbEnable? TRUE:FALSE);
	}

	return S_OK;
}
