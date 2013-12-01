/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.3.1
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/******************************************************************************
Module:  VarConv.cpp
Notices: Copyright (c) 2006 Bruce Liang
Purpose: ���� Variant ������������ͨ�������͵�ת��.
Desc:
Usage:
Rule:	
Example:								
******************************************************************************/

#include "stdafx.h"
#include "VarConv.h"

CString GetComErrorDesc(_com_error e)
{
	HRESULT code = e.Error();
	if(code == TYPE_CAST_ERROR)
		return _T("[VARIANT CAST] type doesn't match");
	else
	{
		IErrorInfo* pei = e.ErrorInfo();
		if(pei)
			return (LPCTSTR)e.Description();
		else
			return e.ErrorMessage();
	}
}

/*########################################################################
			  ------------------------------------------------
							    ��ֵ����ת��
			  ------------------------------------------------
  ########################################################################*/
COleDateTime vartodate(const _variant_t& var)
{
	COleDateTime value;
	switch (var.vt) 
	{
	case VT_DATE:
		value = var.date;
		break;
	case VT_EMPTY:
	case VT_NULL:
		value.SetStatus(COleDateTime::null);
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}
	return value;
}

COleCurrency vartocy(const _variant_t& var)
{
	COleCurrency value;
	switch (var.vt) 
	{
	case VT_CY:
		value = (CURRENCY)var.cyVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		value.m_status = COleCurrency::null;
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}
	return value;
}

bool vartobool(const _variant_t& var)
{
	bool value = false;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal ? true : false;
		break;
	case VT_NULL:
	case VT_EMPTY:
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}
	return value;
}

BYTE vartoi1(const _variant_t& var)
{
	BYTE value = 0;
	switch (var.vt)
	{
	case VT_I1:
	case VT_UI1:
		value = var.bVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}	
	return value;
}

short vartoi2(const _variant_t& var)
{
	short value = 0;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_I1:
	case VT_UI1:
		value = var.bVal;
		break;
	case VT_I2:
	case VT_UI2:
		value = var.iVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}	
	return value;
}

long vartoi4(const _variant_t& var)
{
	long value = 0;
	switch (var.vt)
	{
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_I1:
	case VT_UI1:
		value = var.bVal;
		break;
	case VT_I2:
	case VT_UI2:
		value = var.iVal;
		break;
	case VT_I4:
	case VT_UI4:
		value = var.lVal;
		break;
	case VT_INT:
	case VT_UINT:
		value = var.intVal;
		break;
	case VT_R4:
		value = (long)(var.fltVal);
		break;
	case VT_R8:
		value = (long)(var.dblVal);
		break;
	case VT_DECIMAL:
		value = (long)var;
		break;
	case VT_CY:
		value = (long)var;
		break;
	case VT_LPSTR:		//�ַ���
		value = atol((LPCSTR)(_bstr_t)var);
		break;
	case VT_BSTR:		//�ַ���
	case VT_LPWSTR:		//�ַ���
		value = _wtol((LPCWSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}
	return value;
}

double vartof(const _variant_t& var)
{
	double value = 0;
	switch (var.vt)
	{
	case VT_R4:
		value = var.fltVal;
		break;
	case VT_R8:
		value = var.dblVal;
		break;
	case VT_DECIMAL:
		value = (double)var;
		break;
	case VT_CY:
		value = (double)var;
		break;
	case VT_BOOL:
		value = var.boolVal;
		break;
	case VT_I1:
	case VT_UI1:
		value = var.bVal;
		break;
	case VT_I2:
	case VT_UI2:
		value = var.iVal;
		break;
	case VT_I4:
	case VT_UI4:
		value = var.lVal;
		break;
	case VT_INT:
	case VT_UINT:
		value = var.intVal;
		break;
	case VT_LPSTR:		//�ַ���
		value = atof((LPCSTR)(_bstr_t)var);
		break;
	case VT_BSTR:		//�ַ���
	case VT_LPWSTR:		//�ַ���
		value = _wtof((LPCWSTR)(_bstr_t)var);
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = 0;
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}
	return value;
}

CString vartostr(const _variant_t &var)
{
	CString value;
	
	switch (var.vt)
	{
	case VT_LPSTR:		//�ַ���
	case VT_BSTR:		//�ַ���
	case VT_LPWSTR:		//�ַ���
		value = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I1:
	case VT_UI1:
		value.Format(_T("%d"), var.bVal);
		break;
	case VT_I2://������
		value.Format(_T("%d"), var.iVal);
		break;
	case VT_INT://����
		value.Format(_T("%d"), var.intVal);
		break;
	case VT_I4: //����
		value.Format(_T("%d"), var.lVal);
		break;
	case VT_I8: //������
		value.Format(_T("%d"), var.lVal);
		break;
	case VT_UI2://�޷��Ŷ�����
		value.Format(_T("%u"), var.uiVal);
		break;
	case VT_UINT://�޷�������
		value.Format(_T("%u"), var.uintVal);
		break;
	case VT_UI4: //�޷�������
		value.Format(_T("%u"), var.ulVal);
		break;
	case VT_UI8: //�޷��ų�����
		value.Format(_T("%u"), var.ulVal);
		break;
	case VT_VOID:
		value.Format(_T("%8x"), var.byref);
		break;
	case VT_R4://������
		value.Format(_T("%.4f"), var.fltVal);
		break;
	case VT_R8://˫������
		value.Format(_T("%.8f"), var.dblVal);
		break;
	case VT_DECIMAL: //С��
		value.Format(_T("%.8f"), (double)var);
		break;
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			value = cy.Format();
		}
		break;
	case VT_BOOL://������
		value = var.boolVal ? _T("1") : _T("0");
		break;
	case VT_DATE: //������
		{
			COleDateTime da = COleDateTime(var.date); 
			value = da.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		break;
	case VT_NULL:
	case VT_EMPTY:
		value = _T("");
		break;
	default:
		_com_error e(TYPE_CAST_ERROR);
		throw e;
	}
	return value.Trim();
}