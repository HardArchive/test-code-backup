// LoginAx.cpp : CLoginAx ��ʵ��
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

	HWND hWndUserNameEdit = ::GetDlgItem(m_hWnd, IDC_EDIT_USER);	//��ȡ�û��������Ĵ��ھ��
	HWND hWndPasswordEdit = ::GetDlgItem(m_hWnd, IDC_EDIT_PWD);		//��ȡ���������Ĵ��ھ��

	WCHAR tcUserName[STR_COUNT] = {0};
	::GetWindowTextW(hWndUserNameEdit, tcUserName, STR_COUNT);		//��ȡ������û���
	bstrUserName = tcUserName;

	WCHAR tcPassword[STR_COUNT] = {0};
	::GetWindowTextW(hWndPasswordEdit, tcPassword, STR_COUNT);		//��ȡ���������
	bstrPassword = tcPassword;

	if (m_varbJudgeNull)
	{
		if (bstrUserName == L"")		//�ж��û����Ƿ�Ϊ��
		{
			MessageBox(_T("�û�������Ϊ�գ�"));
		}
		else if (bstrPassword == L"")	//�ж������Ƿ�Ϊ��
		{
			MessageBox(_T("���벻��Ϊ�գ�"));
		}
		else
		{
			Fire_OnLogin(bstrUserName, bstrPassword);	//������¼�¼�
		}
	}
	else
	{
		Fire_OnLogin(bstrUserName, bstrPassword);		//������¼�¼�
	}

	return 0;
}

STDMETHODIMP CLoginAx::SetText(BSTR bstrUserName, BSTR bstrPassword, BSTR bstrLogin)
{
	//��ȡ�û����ı��������ı�����¼��ť�Ĵ��ھ��
	HWND hWndUserNameText = ::GetDlgItem(m_hWnd, IDC_STATIC_USER);
	HWND hWndPasswordText = ::GetDlgItem(m_hWnd, IDC_STATIC_PWD);
	HWND hWndLoginText = ::GetDlgItem(m_hWnd, IDC_BTN_LOGIN);

	//�����û����ı��������ı�����¼��ť����
	::SetWindowTextW(hWndUserNameText, bstrUserName);
	::SetWindowTextW(hWndPasswordText, bstrPassword);
	::SetWindowTextW(hWndLoginText, bstrLogin);

	return S_OK;
}
