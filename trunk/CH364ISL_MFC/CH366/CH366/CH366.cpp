// CH366.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <tchar.h>
//#include "lib/CH364PGMDLL.H"
//#include <Shellapi.h>
#include "lib/CH364PGM.h"

//#pragma comment(lib,"Lib/CH364PGMDLL")
//
#pragma comment(lib,"Powrprof.lib")

extern "C" { 
#include <powrprof.h> 
}

typedef	struct	_CH366_IO_REG {	         // CH364оƬ�����üĴ���
	UCHAR CH366CfgDout;                // 00H ����Ĵ���
	UCHAR CH366CfgCtrl;                // 01H ���ƼĴ���
	UCHAR CH366CfgDin;                 // 02H ����Ĵ���
	UCHAR CH366CfgSet;                 // 03H ���üĴ���
	ULONG rev[61];
	UCHAR CH366CfgAux;				    // F8H �����Ĵ���
} mCH366_IO_REG, *mPCH366_IO_REG;



HANDLE g_hDrv = NULL;   //�������
HMODULE g_hDll = NULL;        //CH364PGMDLL.dll��̬���� 

ULONG CH366ID = 0;
ULONG ISCH366 = 0;
BOOL  g_Ch366Open = FALSE;                  //CH366оƬ��״̬
mPCH366_IO_REG g_pstuCfgRegBaseAdd = NULL;    //CH366оƬ�����üĴ���ָ��

//�Զ���TRACE����
extern "C" __declspec(dllexport) void MyTRACE(WCHAR *fmt, ...)
{
//#ifdef _DEBUG
	WCHAR out[1024];
	va_list body;
	va_start(body,fmt);
	vswprintf_s(out,fmt,body);
	va_end(body);
	OutputDebugStringW(out);
//#endif
}
//��ʾ������Ϣ����
void MyShowError(void)
{
	ULONG ulError = 0;
	TCHAR szErrorMsg[128] = {0}, szOutput[256] = {0};

	ulError = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, ulError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szErrorMsg, sizeof(szErrorMsg), 0);
	//FormatMessage���ܾ��ǽ�GetLastError�õ��Ĵ�����Ϣ�����������Ϣ�����ִ��ţ�ת�����ַ�����Ϣ�ĺ��� 

	_stprintf(szOutput, L"�������:0x%x:%s", ulError, szErrorMsg);
	::MessageBox(NULL, szOutput, L"TIPTOP_CH366", MB_ICONWARNING | MB_OK);
}

//�ر�366�豸
extern "C" __declspec(dllexport) void CloseCH366(void)
{
	if (g_hDrv)
	{
		CloseCH364IODrv(g_hDrv);
	}	

	ReleaseCh364PGM();

	g_hDrv = NULL;
	g_hDll = NULL;
	CH366ID = 0;
	ISCH366 = 0;
	g_Ch366Open = FALSE;
	g_pstuCfgRegBaseAdd = NULL;    //CH366оƬ�����üĴ���ָ��
}


//����366�� �ɹ�����1ʧ�ܷ���0
int FindCard(void)
{	
	CH366ID = 0;
	ISCH366 = 0;
	if(g_hDrv)
	{
		if((FindCH366(g_hDrv, &CH366ID, &ISCH366, &g_pstuCfgRegBaseAdd))==FALSE)
		{
#ifdef _DEBUG
			OutputDebugString(L"FindCard_û���ҵ�CH364�����߳����ظ���\r\n");
#endif	
			MessageBox( NULL, L"FindCard_û���ҵ�CH364�����߳����ظ���", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
			g_Ch366Open = FALSE;
			return 0;    //�Ҳ���CH366��
		}
		else
		{
			if(ISCH366)
			{
				g_Ch366Open = TRUE;
				return 1;
			}
			else
			{
#ifdef _DEBUG
				OutputDebugString(L"FindCard_û���ҵ�CH364��\r\n");
#endif	
				MessageBox( NULL, L"FindCard_û���ҵ�CH364��", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
				g_Ch366Open = FALSE;
				return 0;
			}
		}
	}	

	return 0;
}

/******************************************************************************* 
1�� �������ƣ� OpenTP366
2�� ���������� ��366�豸
3�� ��������� ��
4�� �� �� ֵ�� -1   DLL����ʧ��
			 -2    ��������ʧ��
			 -3    δ�ҵ���
			 1     �򿪳ɹ� 
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-9-1 14:42:46
8�� ��    ע�� 
********************************************************************************/ 
extern "C" __declspec(dllexport) int OpenCH366(void)
{	
	if (g_Ch366Open&&g_hDll&&g_hDrv)
	{
#ifdef _DEBUG
		OutputDebugString(L"���Ѿ��򿪡����������ٴδ�\r\n");
#endif	
		return 1;
	}
	CloseCH366();

	//��Ҫʹ��ch364GPMDLL����Ҫ�ȼ���	

	if (!Init364PGMDLL())
	{
		return -1;
	}

	if(!(SearchPath(NULL, L"CH364PGM", L".sys", NULL, NULL, NULL)))
	{
#ifdef _DEBUG
		OutputDebugString(L"�ڵ�ǰĿ¼��û�з���CH364PGM.sys�ļ�\r\n");
#endif	
		MessageBox(NULL, L"�ڵ�ǰĿ¼��û�з���CH364PGM.sys�ļ�", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		CloseCH366();
		return -2;
	}

	//��ʹ��CH364����֮ǰ���ô˺���������ʼ������,��ʼ���ɹ��󷵻�һ����Ч�ľ��
	g_hDrv = InitCH364IODrv(NULL);
	if(NULL==g_hDrv)
	{
#ifdef _DEBUG
		OutputDebugString(L"װ��CH364IO.sys�ļ�ʧ��\r\n");
#endif	
		MessageBox( NULL, L"װ��CH364IO.sys�ļ�ʧ��", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		CloseCH366();

		return -2;
	}

	if(FindCard()==0) 
	{
#ifdef _DEBUG
		OutputDebugString(L"�Ҳ���CH366��\r\n");
#endif		
		CloseCH366();
		return -3;
	}
	return 1;
}

/******************************************************************************* 
1�� �������ƣ� GetNet
2�� ���������� �õ���ǰ����������
3�� ���������  ��
4�� �� �� ֵ�� ����2Ϊ������1Ϊ���� oΪʧ��
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-9-1 15:09:16
8�� ��    ע�� 
********************************************************************************/ 
extern "C" __declspec(dllexport) int GetNet(void)
{
	UCHAR uchRead = 0;
	PVOID pOffset = 0;

	if(NULL == g_hDrv)
	{	
#ifdef _DEBUG
		OutputDebugString(L"Error : ChangeNet handle == NULL ");
#endif	
		CloseCH366();
		if (1 != OpenCH366())
		{
			::MessageBoxW(NULL, L"��ȡ����״̬ʱ��ǰ��״̬����ȷ�����´�ʧ�ܣ���ȷ�Ͽ��Ѿ���á���", L"TIPTOP_CH366", MB_OK);
			return 0; //��ȡ���绷��ʧ��
		}
	}

	if (!CH364mReadIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgCtrl, &uchRead))
	{
		MessageBox(NULL, L"��ȡ���绷��ʧ�ܣ�", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return 0;
	}
	MyTRACE(L"#####��ȡ���绷��g_pstuCfgRegBaseAdd->CH366CfgCtrl����ֵuchRead��0x%X#########################", uchRead);

	if((uchRead&0x2)==0)     
	{
		return 1;      //��������
	}
	else
	{			
		return 2;      //��������	
	}
}

//�ж�ϵͳ�汾
BOOL IsWIN32_NT(void)
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	return osv.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

//���ػ���Ԥ������  ȡ��Ȩ��
BOOL PrepareShutDown(void)
{
	//  �˴������õĺ������ΪAPI������
	HANDLE hToken = NULL;                   //������йصķ��ʱ�Ǿ����
	HANDLE hProcess =  NULL;				//������̾����	
	LPVOID lpMsgBuf = NULL;
	TOKEN_PRIVILEGES  tp;			// �������ȼ���ǽṹ���һ��ʵ����
	LUID luid;						//  �������ID�ṹ���һ��ʵ��;
	if(!IsWIN32_NT())
	{
		::MessageBox(0, L"��ǰϵͳΪ��WIN_32ϵͳ", L"TIPTOP_CH366", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	hProcess = GetCurrentProcess();
	if(!hProcess)
	{
		::MessageBox(0, L"��ȡ��ǰ���̾��ʧ�ܡ���", L"TIPTOP_CH366", MB_OK);
		return FALSE;
	}
	//  �õ�һ��������йصķ��ʱ�Ǿ����	
	if(!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{	
		MyShowError();
		::LocalFree(lpMsgBuf);
		return FALSE;
	}		

	//  ��luid����ֵ��
	if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid))  
	{
		MyShowError();
		::LocalFree(lpMsgBuf);
		return FALSE;
	}	

	tp.PrivilegeCount = 1;       //  ���ṹ��ʵ����ֵ��
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		MyShowError();
		::LocalFree(lpMsgBuf);
		return FALSE;
	}	
	return TRUE;
}

//EWX_FORCE  ǿ��ע��
//����ǿ����ֹ���̡�
//	���˱�־���ã�Windows���ᷢ����ϢWM_QUERYENDSESSION��WM_ENDSESSION����Ϣ��Ŀǰ��ϵͳ�����еĳ�������ܻᵼ��Ӧ�ó���ʧ���ݡ�
//	��ˣ���Ӧ��ֻ�ڽ��������ʹ�ô˱�־��
//EWX_LOGOFF �����ر����н��̣�Ȼ��ע���û���
//EWX_POWEROFF ����
//	�ر�ϵͳ���رյ�Դ����ϵͳ����֧�ֶϵ硣 ����
//	WindowsҪ�� ����Windows NT�е��ý��̱����� SE_SHUTDOWN_NAME ��Ȩ�� ����Windows 9X�У�����ֱ�ӵ��á�
//EWX_REBOOT ����
//	�ر�ϵͳ��Ȼ����������ϵͳ�� ����
//	WindowsҪ�� ����Windows NT�У����ý��̱�����SE_SHUTDOWN_NAME��Ȩ�� ����Windows 9X�У�����ֱ�ӵ��á�
//EWX_SHUTDOWN ����
//	�ر�ϵͳ����ȫ�عرյ�Դ�������ļ��������Ѿ�ˢ�µ������ϣ������������еĽ����Ѿ�ֹͣ�� ���� //�̻�
//	WindowsҪ�� ����Windows NT�У����ý��̱�����SE_SHUTDOWN_NAME��Ȩ�� ����Windows 9X�У�����ֱ�ӵ��á�
//�̻� paternity machine
//�ػ�ע��������������
BOOL PoweroffEx(UINT uFlags)
{
	LPVOID lpMsgBuf = NULL;
	if(0==ExitWindowsEx(uFlags, 0))
	{	
		MyShowError();
		//OutputDebugString(lpMsgBuf);
		::LocalFree(lpMsgBuf);
		return FALSE;
	}
	return TRUE;
}


//����
BOOL Reboot(void)
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, L"PrepareShutDown", L"SHUTDOWN", MB_OK);	
		return FALSE;
	}

	if (!PoweroffEx(EWX_REBOOT))
	{
		return FALSE;
	}

	return TRUE;
}

//�رռ����
BOOL ShutDown(void) 
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, L"PrepareShutDown", L"SHUTDOWN", MB_OK);	
		return FALSE;
	}
	if (!PoweroffEx(EWX_SHUTDOWN))
	{
		return FALSE;
	}
	return TRUE;
}

//����
BOOL Hibernate(void)
{
	BOOL bRet = FALSE;                    //����ֵ

	if (!IsPwrHibernateAllowed())
	{
		::MessageBoxW(NULL, L"��������ϵͳ���߹��ܡ���", L"TIPTOP_366", MB_OK);
		return FALSE;
	}

	if(!PrepareShutDown())
	{
		MessageBox(NULL, L"PrepareShutDown", L"SHUTDOWN", MB_OK);	
		return FALSE;
	}

	if(!IsWIN32_NT())
	{
		::MessageBox(0, _T("��ǰϵͳΪ��WIN_32ϵͳ"), _T("Error"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	//if (!SetSuspendState(1,0,0))
	//{
	//	return FALSE;
	//}

	//SetThreadExecutionState(ES_DISPLAY_REQUIRED);
	//Sleep(1000);
	//CloseCH366();
	//OpenCH366();

	return SetSuspendState(1,0,0);	
}

/******************************************************************************* 
1�� �������ƣ� ChangeNet
2�� ���������� 
3�� ��������� bNetBit    Ϊ1ʱ���л�������;�����л���������
			 iPowerOff    Ϊ1ʱ�������������Ϊ2���߼��������������ߵĻ�����������Զ����� Ϊ3ʱ�رռ����
			 bInterFace Ϊ1ʱ������ʱ����ѡ����棬���򲻳�ѡ����档
4�� �� �� ֵ��  0���ɹ��л�
			   1��δ�ҵ���
			   2����ȡ���绷��ʧ��
			   3�������л���������ʧ��
			   4�������л�д�뻻������ʧ��
			   5��������������
			   6�����������ߡ��ػ�����
			   7���л��������ͳ���
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-9-23 17:48:03
8�� ��    ע�� �˴�δ�Ӵ����������
********************************************************************************/ 
extern "C" __declspec(dllexport) int ChangeNet(BOOL bNetBit, int iPowerOff, BOOL bInterFace)
{
	int  iNetStatue = 0;
	UCHAR uchRead = 0;
	if (!g_Ch366Open)
	{
		CloseCH366();
		//Sleep(500);
		if (1 != OpenCH366())
		{
			return 1;
		}
	}

	//�ж������л��Ƿ���ȷ
	iNetStatue = GetNet();
	if (1 == iNetStatue)  //��ǰΪ����
	{
		if (!bNetBit)
		{
			::MessageBox(NULL, L"��ǰ�����Ѿ�Ϊ��������", L"TIPTOP_366", MB_OK);	   	     
			return 7;
		}
		//uchRead = 0x00;
		uchRead = 0xA8;
	}
	else if (2 == iNetStatue)  //��ǰΪ����
	{
		if (bNetBit)
		{
			::MessageBox(NULL, L"��ǰ�����Ѿ�Ϊ��������", L"TIPTOP_366", MB_OK);	   	     
			return 7;
		}
		//uchRead = 0xA;
		uchRead = 0x92;
	}
	else
	{
		return 2;  //��ȡ���绷��ʧ��
	}

	//��ȡ���绷��
	if(!CH364mReadIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgCtrl, &uchRead))
	{
		MessageBox(NULL, L"��CH366��ȡ���绷������!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
		return 2;
	}
	MyTRACE(L"�س�ʼ�����üĴ���&g_pstuCfgRegBaseAdd->CH366CfgCtrl��������,uchRead��0x%X", uchRead);

	if(IDOK == MessageBox(NULL, L"�Ƿ�Ҫ�л���ǰ���绷��? \n(�л�ǰ�����������������߼����!)\n", 
		L"TIPTOP_CH366", MB_OKCANCEL | MB_ICONQUESTION ))
	{
		uchRead = uchRead | 0x80;

		if( bNetBit == 1 )
			uchRead = uchRead | 0x08;		//λ3��1��ϵͳ�ػ���̵�����ѡ���л�������
		else
			uchRead = uchRead & 0x0F7;    //λ3��0��ϵͳ�ػ���̵�����ѡ���л�������

		if(bInterFace)
		{
			uchRead = uchRead & 0x0CF;	   //λ4��λ5��ͬʱ������ѡ����棬����ȫ����Ϊ0
		}
		else
		{
			if( bNetBit == 1 )	          //λ5��λ3��ͬ,λ4��λ3�෴ʱ����ѡ�����
				uchRead = uchRead | 0x20;
			else
				uchRead = uchRead | 0x10;
		}
		MyTRACE(L"�����л����뿨���л�������bNetBit��%d;iPowerOff:%d;bInterFace:%d;", bNetBit, iPowerOff, bInterFace);
		MyTRACE(L"���üĴ���&g_pstuCfgRegBaseAdd->CH366CfgCtrlд������,uchRead��0x%X", uchRead);

		//���л���Ϣд����뿨 
		if( !CH364mWriteIoByte( g_hDrv,&g_pstuCfgRegBaseAdd->CH366CfgCtrl,(UCHAR)uchRead) )
		{
			MessageBox(NULL, L"дCH366IO���ݳ���!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
			return 3;
		}

		if(2 == iPowerOff) //�����л�   ����Ӧ����д�뻻������
		{	
			//uchRead = 0xB8;
			if(!CH364mReadIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgAux, &uchRead))
			{
				//_stprintf( buf, L"Write before CH366CfgAux:%02X "), ch );
				//MessageBox( NULL, buf, L"TIPTOP_CH366", MB_ICONWARNING | MB_OK );
				MessageBox(NULL, L"��CH366�����Ĵ������ݳ���!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return 3;
			}
			MyTRACE(L"�����Ĵ���&g_pstuCfgRegBaseAdd->CH366CfgAux��ȡ���ݣ�uchRead��0x%X", uchRead);

			uchRead = uchRead & 0x0FE ;
			MyTRACE(L"�����Ĵ���&g_pstuCfgRegBaseAdd->CH366CfgAuxд�����ݣ�uchRead��0x%X", uchRead);
			if(!CH364mWriteIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgAux, (UCHAR)uchRead))
			{
				MessageBox(NULL, L"дCH364IO����!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return 4;
			}
		}

		Sleep(500);
		if(1 == iPowerOff)
		{
			if (!Reboot())
			{
				return 6;
			}

		}
		else if (2 == iPowerOff)
		{
			if (!Hibernate())
			{
				return 6;
			}
		}
		else if (3 == iPowerOff)
		{
			if (!ShutDown())
			{
				return 6;
			}				
		}
		else
		{
			return 5;
		}				
	}

	return 0;
}
