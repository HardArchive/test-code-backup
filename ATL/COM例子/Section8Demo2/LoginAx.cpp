// LoginAx.cpp : CLoginAx 的实现
#include "stdafx.h"
#include "LoginAx.h"


// CLoginAx
#define STR_COUNT 100

STDMETHODIMP CLoginAx::get_JudgeNull(VARIANT_BOOL* pVal)
{
	*pVal = m_varbJudgeNull;

	return S_OK;
}

STDMETHODIMP CLoginAx::put_JudgeNull(VARIANT_BOOL newVal)
{
	m_varbJudgeNull = newVal;

	return S_OK;
}

LRESULT CLoginAx::OnBnClickedBtnLogin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CComBSTR bstrUserName;
	CComBSTR bstrPassword;

	HWND hWndUserNameEdit = ::GetDlgItem(m_hWnd, IDC_EDIT_USER);	//获取用户名输入框的窗口句柄
	HWND hWndPasswordEdit = ::GetDlgItem(m_hWnd, IDC_EDIT_PWD);		//获取密码输入框的窗口句柄

	WCHAR tcUserName[STR_COUNT] = {0};
	::GetWindowTextW(hWndUserNameEdit, tcUserName, STR_COUNT);		//获取输入的用户名
	bstrUserName = tcUserName;

	WCHAR tcPassword[STR_COUNT] = {0};
	::GetWindowTextW(hWndPasswordEdit, tcPassword, STR_COUNT);		//获取输入的密码
	bstrPassword = tcPassword;

	if (m_varbJudgeNull)
	{
		if (bstrUserName == L"")		//判断用户名是否为空
		{
			MessageBox(_T("用户名不能为空！"));
		}
		else if (bstrPassword == L"")	//判断密码是否为空
		{
			MessageBox(_T("密码不能为空！"));
		}
		else
		{
			Fire_OnLogin(bstrUserName, bstrPassword);	//触发登录事件
		}
	}
	else
	{
		Fire_OnLogin(bstrUserName, bstrPassword);		//触发登录事件
	}

	return 0;
}

STDMETHODIMP CLoginAx::SetText(BSTR bstrUserName, BSTR bstrPassword, BSTR bstrLogin)
{
	//获取用户名文本，密码文本，登录按钮的窗口句柄
	HWND hWndUserNameText = ::GetDlgItem(m_hWnd, IDC_STATIC_USER);
	HWND hWndPasswordText = ::GetDlgItem(m_hWnd, IDC_STATIC_PWD);
	HWND hWndLoginText = ::GetDlgItem(m_hWnd, IDC_BTN_LOGIN);

	//设置用户名文本，密码文本，登录按钮文字
	::SetWindowTextW(hWndUserNameText, bstrUserName);
	::SetWindowTextW(hWndPasswordText, bstrPassword);
	::SetWindowTextW(hWndLoginText, bstrLogin);

	return S_OK;
}
