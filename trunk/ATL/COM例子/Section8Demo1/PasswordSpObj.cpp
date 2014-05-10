// PasswordSpObj.cpp : CPasswordSpObj 的实现

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
	//判断验证连续失败的次数是否超过指定的次数
	if (m_lErrorCount<m_lErrorCountAllowed)
	{
		CComBSTR bstrA;
		CComBSTR bstrB;
		bstrA.AssignBSTR(bstrUser);
		bstrB.AssignBSTR(bstrPwd);

		//比较用户名
		if (bstrA == m_bstrUser)
		{
			//比较密码
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
		CComBSTR bstrMsg = L"系统拒绝继续验证用户！";
		Fire_OnMsg(bstrMsg);
	}

	return S_OK;
}
