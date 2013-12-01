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

Module:  VarConv.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: ���� Variant ����ͨ�������͵�ת������.
Desc:
			1. ͨ���������ݿ���ʻ������� COM �йصĲ���

Usage:

Rule:	

Example:	(��: TestDBAccessor ���Գ������е����ݿ���ʲ���)
											
******************************************************************************/

/*########################################################################
			  ------------------------------------------------
							 Variant ��ֵ����ת��
			  ------------------------------------------------
  ########################################################################*/

#pragma once

#include <comdef.h>
#include <comutil.h>

#define TYPE_CAST_ERROR		MAKE_HRESULT(1, 0xFF, 0xFF)

// ��ȡ _com_error ����
CString GetComErrorDesc(_com_error e);

// Variant ��ֵ����ת��
double			vartof		(const _variant_t& var);
BYTE			vartoi1		(const _variant_t& var);
short			vartoi2		(const _variant_t& var);
long			vartoi4		(const _variant_t& var);
COleCurrency	vartocy		(const _variant_t& var);
CString			vartostr	(const _variant_t& var);
bool			vartobool	(const _variant_t& var);
COleDateTime	vartodate	(const _variant_t& var);
