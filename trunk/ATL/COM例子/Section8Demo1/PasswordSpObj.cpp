// PasswordSpObj.cpp : CPasswordSpObj ��ʵ��

#include "stdafx.h"
#include "PasswordSpObj.h"


// CPasswordSpObj


STDMETHODIMP CPasswordSpObj::get_ErrorCountAllowed(LONG* pVal)
{
	*pVal = m_lErrorCountAllowed;
	return S_OK;
}

STDMETHODIMP CPasswordSpObj::put_ErrorCountAllowed(LONG newVal)
{
	m_lErrorCountAllowed = newVal;
	return S_OK;
}

STDMETHODIMP CPasswordSpObj::CheckPassword(BSTR bstrUser, BSTR bstrPwd, VARIANT_BOOL* pvarbResult)
{
	//�ж���֤����ʧ�ܵĴ����Ƿ񳬹�ָ���Ĵ���
	if (m_lErrorCount<m_lErrorCountAllowed)
	{
		CComBSTR bstrA;
		CComBSTR bstrB;
		bstrA.AssignBSTR(bstrUser);
		bstrB.AssignBSTR(bstrPwd);

		//�Ƚ��û���
		if (bstrA == m_bstrUser)
		{
			//�Ƚ�����
			if (bstrB == m_bstrPwd)
			{
				*pvarbResult = VARIANT_TRUE;
				m_lErrorCount = 0;
			}
			else
			{
				*pvarbResult = VARIANT_FALSE;
				++m_lErrorCount;
			}
		}
		else
		{
			*pvarbResult = VARIANT_FALSE;
			++m_lErrorCount;
		}
	}
	else
	{
		CComBSTR bstrMsg = L"ϵͳ�ܾ�������֤�û���";
		Fire_OnMsg(bstrMsg);
	}

	return S_OK;
}
