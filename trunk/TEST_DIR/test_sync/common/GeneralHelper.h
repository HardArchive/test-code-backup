//file GeneralHelper.h
/***************************************************************************************************
* 1�� File       �� GeneralHelper.h
* 2�� Version    �� *.*
* 3�� Description�� ͨ��ͷ�ļ�  ��Ҫ��һЩ���õ�ͷ �ͺ�
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-26 15:58:21
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __GENERAL_HELPER_H__
#define __GENERAL_HELPER_H__

//�������ͷ�ļ�
#ifndef _WINDOWS_
#include <windows.h>
#endif

//ʹ��string ֧�ֿ�խ�ַ�
#include <string>
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
using namespace std;

//���ú궨��
#if !defined(nullptr)
#define nullptr	NULL
#endif

#include <atlstr.h>
#ifndef ASSERT
#define ASSERT(f)	ATLASSERT(f)
#endif
#ifndef VERIFY
#define VERIFY(f)	ATLVERIFY(f)
#endif


//�ų�����
#pragma warning (disable:4800)  //BOOL  bool

//�������ѵ�ͷ�ļ�
#include "TraceUtil.h"

#endif /*__GENERAL_HELPER_H__*/
