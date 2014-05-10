// HelloCompositeControl.cpp : CHelloCompositeControl ��ʵ��
#include "stdafx.h"
#include "HelloCompositeControl.h"


// CHelloCompositeControl

LRESULT CHelloCompositeControl::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MessageBox(_T("����Button1"));

	if (++m_lBtn1ClickCount%3 == 0)
	{
		//��ؼ������߷����¼�
		Fire_OnClickBtn(0, m_lBtn1ClickCount);
	}

	return 0;
}

LRESULT CHelloCompositeControl::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MessageBox(_T("����Button2"));

	if (++m_lBtn2ClickCount%3 == 0)
	{
		//��ؼ������߷����¼�
		Fire_OnClickBtn(1, m_lBtn2ClickCount);
	}

	return 0;
}

STDMETHODIMP CHelloCompositeControl::get_Description(BSTR* pVal)
{
	//�ͷžɵ��ַ���
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
		//��ȡButton1�Ĵ��ھ��
		HWND hBtn = ::GetDlgItem(m_hWnd, IDC_BUTTON1);

		if (varbEnable)
		{
			//����Button1
			::EnableWindow(hBtn, TRUE);		
		}
		else
		{
			//����Button
			::EnableWindow(hBtn, FALSE);
		}

		//��д�ұ����������漸��  ::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON1), varbEnable? TRUE:FALSE);
	}
	else if (lBtnIndex == 1)
	{
		//��ȡButton2�Ĵ��ھ��
		HWND hBtn = ::GetDlgItem(m_hWnd, IDC_BUTTON2);

		if (varbEnable)
		{
			//����Button2
			::EnableWindow(hBtn, TRUE);
		}
		else
		{
			//����Button2
			::EnableWindow(hBtn, FALSE);
		}

		//��д�ұ����������漸��  ::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON2), varbEnable? TRUE:FALSE);
	}

	return S_OK;
}
