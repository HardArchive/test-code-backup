#include "StdAfx.h"
#include "Ch366.h"

CCh366::CCh366(void)
{
	m_hDrv = NULL;
	m_bIsInit = FALSE;
	m_iIsCardExist = 0;
	m_iIsIntranet = 0;

}

CCh366::~CCh366(void)
{
	m_bIsInit = FALSE;
	m_iIsCardExist = 0;
	m_iIsIntranet = 0;
	if (m_hDrv)
	{
		CloseCH364IODrv(m_hDrv);
	}
	
}

BOOL CCh366::Init()
{
	ULONG CH366ID=0;
	ULONG ISCH366=0;
	//ZeroMemory(&m_stuCfgRegBaseAdd, sizeof(mPCH366_IO_REG));
	//mPCH366_IO_REG CfgRegBaseAdd;

	// ��Ҫʹ��DLL����Ҫ�ȼ���
	m_hDll = LoadLibrary(L"CH364PGMDLL.dll" );
	if (!m_hDll)  // ����DLLʧ��,����δ��װ��ϵͳ��
	{
		MessageBox(NULL, L"�޷�����CH364���������CH364PGMDLL", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	if(!(SearchPath(NULL, L"CH364PGM", L".sys", NULL, NULL, NULL)))
	{
		MessageBox(NULL, L"�ڵ�ǰĿ¼��û�з���CH364PGM.sys�ļ�", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return FALSE;
	}

	//��ʹ��CH364����֮ǰ���ô˺���������ʼ������,��ʼ���ɹ��󷵻�һ����Ч�ľ��.
	if((m_hDrv = InitCH364IODrv(/*hDialog*/NULL))==NULL)
	{
		MessageBox( NULL/*hDialog*/, L"װ��CH364IO.sys�ļ�ʧ��", L"TIPTOP_CH366", MB_ICONSTOP | MB_OK );
		return FALSE;
	}

	CH366ID = 0;
	ISCH366 = 0;

	//BOOL	WINAPI	FindCH366(         //����CH366оƬ,��ȡ�����Ϣ
	//	HANDLE          DrvHandle,     //��ʼ������ʱ���ص���Ч���
	//	PULONG          CH364_ID,      //CH366оƬ��λ�ú�,��������ָ����ʼоƬλ�ú�
	//	PULONG          IS_CH366,           
	//	PVOID           CfgRegBaseAddr);//CH366���üĴ�����ַ
	if((FindCH366(m_hDrv, &CH366ID, &ISCH366, /*&m_stuCfgRegBaseAdd*/&m_pstuCfgRegBaseAdd)) == FALSE)
	{
		m_iIsCardExist = 2;
		MessageBox(NULL, L"û���ҵ�CH364��\n���߳����ظ���", L"TIPTOP_CH366", MB_ICONSTOP | MB_OK );
		return FALSE;
	}
	else
	{
		//��ȡоƬ��Ϣ�ɹ����ж��Ƿ�Ϊ366оƬ
		if(ISCH366)
		{
			m_iIsCardExist = 1;
		}else
		{
			m_iIsCardExist = 2;
			MessageBox(NULL, L"û���ҵ�CH364��", L"TIPTOP_CH366", MB_ICONSTOP | MB_OK );
			return FALSE;
		}
	}

	UCHAR uchRead = 0;
	PVOID pOffset = 0;
	//BOOL	WINAPI	CH364mReadIoByte(   // ��I/O�˿ڶ�ȡһ���ֽ�
	//	HANDLE          DrvHandle,
	//	PVOID			iAddr,          // ָ��I/O�˿ڵĵ�ַ
	//	PUCHAR			oByte );        // ָ��һ���ֽڵ�Ԫ,���ڱ����ȡ���ֽ�����//������ȡ��ǰ���绷��
	//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgCtrl;
	//pOffset = (PVOID)&m_pstuCfgRegBaseAdd->CH366CfgCtrl;
 	if (!CH364mReadIoByte(m_hDrv, /*pOffset*/&m_pstuCfgRegBaseAdd->CH366CfgCtrl, &uchRead))
	{
		MessageBox(NULL, L"��ȡ���绷��ʧ�ܣ�", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return FALSE;
	}	

	if((uchRead&0x2)==0)
	{
		m_iIsIntranet = 1;
	}
	else
	{
		m_iIsIntranet = 2;
	}
	m_bIsInit = TRUE;


	////test^^^^^^^^^^^^
	//uchRead = 0;
	//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
	//pOffset = &m_pstuCfgRegBaseAdd->CH366CfgDout;
	//if (!CH364mReadIoByte(m_hDrv, /*pOffset*/&m_pstuCfgRegBaseAdd->CH366CfgDout, &uchRead))
	//{
	//	MessageBox(NULL, L"��ȡ���绷��ʧ�ܣ�", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
	//	return FALSE;
	//}	
	return TRUE;
}

BOOL CCh366::SwitchWork()
{
	//UCHAR t[8] = "";
	PVOID pOffset = 0;
	ULONG datavar = 0;

	HINSTANCE hInsQHuan = 0;	
	DWORD LastResult = 0;

	//mPCH366_IO_REG CfgRegBaseAdd;	
	if(1 == m_iIsCardExist)
	{
		UCHAR ch = 0;
		// ��I/O�˿ڶ�ȡһ���ֽ�
		pOffset = &m_pstuCfgRegBaseAdd->CH366CfgCtrl;
		CH364mReadIoByte(m_hDrv, /*pOffset*/&m_pstuCfgRegBaseAdd->CH366CfgCtrl, &ch); 
		//����������
		if (1 == m_iIsIntranet)
		{
			if((ch&0x2) != 0)
			{
				::MessageBox(NULL, L"��������������!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
			//�������
			//datavar = 0x07;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// ��I/O�˿�д�����

			//�����л�������
			datavar = 0x2a;  
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgCtrl;  //ƫ����			
			CH364mWriteIoByte(m_hDrv, &m_pstuCfgRegBaseAdd->CH366CfgCtrl/*pOffset*/, (UCHAR)datavar);// ��I/O�˿�д��һ���ֽ�
			//�ٴ�����
			//datavar = 0x17;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// ��I/O�˿�д������
		}
		else if (2 == m_iIsIntranet)
		{			
			if((ch&0x2)==0)
			{
				::MessageBox(NULL, L"��������������!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
			//�������
			//datavar = 0x07;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// ��I/O�˿�д�����
			//�����л�������
			datavar=0x10;    
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgCtrl;  //ƫ����
			CH364mWriteIoByte(m_hDrv, &m_pstuCfgRegBaseAdd->CH366CfgCtrl/*pOffset*/, (UCHAR)datavar);
			//�ٴ�����
			//datavar = 0x17;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// ��I/O�˿�д������
		}
		Reboot();
	}
	else
	{
		::MessageBox(NULL, L"���뿨�豸û�ҵ�!",L"TIPTOP_CH366", MB_OK|MB_ICONERROR);
	}
	return TRUE;

}

void CCh366::Reboot()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 
	// Get a token for this process. 
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		::MessageBox(NULL, L"OpenProcessToken", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
	// Get the LUID for the shutdown privilege. 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
	// Cannot test the return value of AdjustTokenPrivileges. 
	if (GetLastError() != ERROR_SUCCESS)
		::MessageBox(NULL, L"AdjustTokenPrivileges", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
	// Shut down the system and force all applications to close. 
	if (!ExitWindowsEx(EWX_REBOOT, 0)) 
		::MessageBox(NULL, L"ExitWindowsEx", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
}

BOOL CCh366::GetCardExist()
{
	if (1 == m_iIsCardExist)
		return TRUE;
	else
		return FALSE;

}

BOOL CCh366::GetNetWork()
{
	if (1 == m_iIsIntranet)
		return TRUE;
	else
		return FALSE;
}