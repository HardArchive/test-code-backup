//file GeneralHelper.h
/***************************************************************************************************
* 1、 File       ： GeneralHelper.h
* 2、 Version    ： *.*
* 3、 Description： 通用头文件  主要有一些常用的头 和宏
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-26 15:58:21
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __GENERAL_HELPER_H__
#define __GENERAL_HELPER_H__

//使用string 支持宽窄字符
#include <string>
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
using namespace std;

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
#include "TraceUtil.h"


#endif /*__GENERAL_HELPER_H__*/
