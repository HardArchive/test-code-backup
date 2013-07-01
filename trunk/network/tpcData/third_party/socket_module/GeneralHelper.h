/*

Optional Macros:

Windows:
++++++++++++++++++++++
_WIN32_WINNT		: Windows NT �汾
WINVER				: Windows �汾
_USE_MFC			: ʹ�� MFC
_WINSOCK_SUPPORT	: ֧�� Windows Socket
_RIBBONS_SUPPORT	: ֧�� Ribbons ������
_DETECT_MEMORY_LEAK	: DEBUG ״̬��֧���ڴ�й¶���

Windows CE:
++++++++++++++++++++++
WINVER				: Windows �汾
_USE_MFC			: ʹ�� MFC
_WINSOCK_SUPPORT	: ֧�� Windows Socket
_DETECT_MEMORY_LEAK	: DEBUG ״̬��֧���ڴ�й¶���
_ONLY_DETECT_CONFIRMED_MEMORY_LEAK_	: ֻ�����ܹ�ȷ�ϵ��ڴ�й¶������ȷ���Ĳ����棩
---------------------------
VC 2010   
	_MSC_VER == 1600
VC 2008   
	_MSC_VER == 1500
VC 2005   
	_MSC_VER == 1400
VC 7.1   
	_MSC_VER == 1310
VC 7.0   
	_MSC_VER == 1300
VC 6.0 
	_MSC_VER == 1200
---------------------------
Windows 7
	_WIN32_WINNT >= 0x0601
	WINVER >= 0x0601
Windows Server 2008
_WIN32_WINNT >= 0x0600
WINVER >= 0x0600
Windows Vista
	_WIN32_WINNT >= 0x0600
	WINVER >= 0x0600
Windows Server 2003
	_WIN32_WINNT >= 0x0502
	WINVER >= 0x0502
Windows XP
	_WIN32_WINNT >= 0x0501
	WINVER >= 0x0501
Windows 2000
	_WIN32_WINNT >= 0x0500
	WINVER >= 0x0500
Windows NT 4.0
	_WIN32_WINNT >= 0x0400
	WINVER >= 0x0400
Windows Me
	_WIN32_WINDOWS=0x0500
	WINVER >= 0x0500
Windows 98
	_WIN32_WINDOWS >= 0x0410
	WINVER >= 0x0410
Windows 95
	_WIN32_WINDOWS >= 0x0400
	WINVER >= 0x0400
---------------------------
*/

#pragma once

#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN						//�� Windows ͷ���ų�����ʹ�õ�����
#endif

#ifndef _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
	#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	//ĳЩ CString ���캯����Ϊ��ʽ��
#endif

#if _MSC_VER >= 1400	//VC 8.0 ���ϰ汾

	#ifdef _DEBUG
		#ifndef _SECURE_SCL
			#define _SECURE_SCL					0
		#endif
		#ifndef _HAS_ITERATOR_DEBUGGING
			#define _HAS_ITERATOR_DEBUGGING		0
		#endif
	#endif

	#ifndef _CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE		1
	#endif

	#ifndef _SCL_SECURE_NO_DEPRECATE
		#define _SCL_SECURE_NO_DEPRECATE		1
	#endif

	#ifndef _ATL_SECURE_NO_WARNINGS
		#define _ATL_SECURE_NO_WARNINGS			1
	#endif

	#ifndef _CRT_NON_CONFORMING_SWPRINTFS
		#define _CRT_NON_CONFORMING_SWPRINTFS	1
	#endif

	#ifndef _SECURE_ATL
		#define _SECURE_ATL						1
	#endif

#endif

#ifdef _WIN32_WCE

	#ifdef _DEBUG
		#define DBG_MSG	TRUE
		#pragma comment(linker, "/nodefaultlib:libcd.lib")
	#else
		#define DBG_MSG	FALSE
		#pragma comment(linker, "/nodefaultlib:libc.lib")
	#endif

	#ifndef WINVER
		#define WINVER	_WIN32_WCE
	#endif

	#ifndef _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA
		#define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA
	#endif

	#ifndef _CE_ACTIVEX
		#define _CE_ACTIVEX
	#endif

	#ifdef _CE_DCOM
		#ifndef _ATL_APARTMENT_THREADED
			#define _ATL_APARTMENT_THREADED
		#endif
	#endif

	#ifndef _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA
		#define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA
	#endif

	#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
		#ifndef SHELL_AYGSHELL
			#define SHELL_AYGSHELL
		#endif

		#ifndef _DEVICE_RESOLUTION_AWARE
			#define _DEVICE_RESOLUTION_AWARE
		#endif

		#ifdef _DEVICE_RESOLUTION_AWARE
			#include "DeviceResolutionAware.h"
		#endif

		#if (_WIN32_WCE < 0x500)
			#pragma comment(lib, "ccrtrtti.lib")
			#ifdef _X86_	
				#if defined(_DEBUG)
					#pragma comment(lib, "libcmtx86d.lib")
				#else
					#pragma comment(lib, "libcmtx86.lib")
				#endif
			#endif
		#endif
	#endif

#else

	#ifndef WINVER
		#ifndef _WIN32_WINNT
			#define WINVER			0x0501
		#else
			#define WINVER			_WIN32_WINNT
		#endif
	#endif

	#ifdef _DETECT_MEMORY_LEAK
		#ifndef _CRTDBG_MAP_ALLOC
			#define _CRTDBG_MAP_ALLOC
		#endif
	#endif

#endif

#if _MSC_VER >= 1600
	#include <SDKDDKVer.h>
#endif

#ifndef _USE_MFC

	#include <Windows.h>
	#include <WindowsX.h>
	#include <commctrl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <tchar.h>
	#include <atlstr.h>
	#include <atltime.h>

	#ifndef ASSERT
		#define ASSERT(f)	ATLASSERT(f)
	#endif
	#ifndef VERIFY
		#define VERIFY(f)	ATLVERIFY(f)
	#endif

	//#ifndef TRACE
	//	#include <atltrace.h>

	//	#define TRACE							AtlTrace
	//	#define TRACE0(f)						TRACE(f)
	//	#define TRACE1(f, p1)					TRACE(f, p1)
	//	#define TRACE2(f, p1, p2)				TRACE(f, p1, p2)
	//	#define TRACE3(f, p1, p2, p3)			TRACE(f, p1, p2, p3)
	//	#define TRACE4(f, p1, p2, p3, p4)		TRACE(f, p1, p2, p3, p4)
	//	#define TRACE5(f, p1, p2, p3, p4, p5)	TRACE(f, p1, p2, p3, p4, p5)
	//#endif

#else

	#ifndef _AFX_ALL_WARNINGS
		#define _AFX_ALL_WARNINGS
	#endif

	#include <afxwin.h>				//MFC core and standard components
	#include <afxext.h>				//MFC extensions
	#include <afxdisp.h>			//MFC Automation classes

	#ifndef _AFX_NO_OLE_SUPPORT
		#include <afxdtctl.h>		//MFC support for Internet Explorer 4 Common Controls
	#endif

	#ifndef _AFX_NO_AFXCMN_SUPPORT
		#include <afxcmn.h>			//MFC support for Windows Common Controls
	#endif

	#ifdef _RIBBONS_SUPPORT
		#include <afxcontrolbars.h>	//MFC support for ribbons and control bars
	#endif

#endif

#ifdef _WINSOCK_SUPPORT
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <mswsock.h>
#endif

#ifdef _WIN32_WCE
	#include <ceconfig.h>
	#include <altcecrt.h>
#endif

#include <atlbase.h>
#include <atlconv.h>

#include "Singleton.h"
#include "Event.h"
#include "Semaphore.h"
#include "CriticalSection.h"
#include "STLHelper.h"
//#include "Win32Helper.h"
#include "PrivateHeap.h"
//#include "bufferptr.h"

#if defined (_DEBUG) && defined (_DETECT_MEMORY_LEAK)
	#if defined (_WIN32_WCE)
		#include "debug/ce_crtdbg.h"
	#else
		#include "debug/win32_crtdbg.h"
	#endif
#endif
