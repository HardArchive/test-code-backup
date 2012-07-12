//������Ȩ�޽��� 
//
//Author:  sinister
//Email:   sinister@whitecell.org
//Homepage:http://www.whitecell.org 
//Date:    2006-02-12

/*****************************************************************
�ļ���        : wssrun.c
����          : ������Ȩ�޽���
����          : sinister
����޸�����  : 2006.2.09

*****************************************************************/


// 
// д���������Ϊ���� Windows ������������Խ�����һЩ����
// �ͽ������̣��� pslist/pskill ֮�๤���޷�������������
// �������ḻ����Ϣ�����������л����鷳�ĺܡ������д������
// ����������������У�ʹ�� SYSTEM ���� TOKEN �滻���ﵽĿ�ġ�
// ��������ͨ���Բ��ã��͸��������ַ������˷�������ʹ regedit 
// �鿴���༭ SAM ��ע���������ֶ���Ϊ��
//
// wssrun taskmgr.exe
// wssrun regedit.exe
//


#include <Windows.h>
#include <AccCtrl.h>
#include <TlHelp32.h>
#include <AclAPI.h>
#include <Shlwapi.h>
//#include 
//#include 

#pragma comment(lib,"Shlwapi.lib")


/////////////////////////////////////////////////////////////////
// �������� :�Զ��幤�ߺ���
// ����ģ�� : 
////////////////////////////////////////////////////////////////
// ���� :������ǰ����Ȩ��
// ע�� :
/////////////////////////////////////////////////////////////////
// ���� : sinister
// �����汾 : 1.00.00
// �������� : 2006.2.09
/////////////////////////////////////////////////////////////////
// ��   ��   ��   ��   ��   ʷ
////////////////////////////////////////////////////////////////
// �޸��� :
// �޸����� :
// �޸����� :
/////////////////////////////////////////////////////////////////
//LPCTSTR szPrivilege  "SeDebugPrivilege" //���ʳ��򼶽�������
BOOL EnableDebugPriv( LPCTSTR szPrivilege )
{
	HANDLE hToken = NULL;
	LUID sedebugnameValue = {0};
	TOKEN_PRIVILEGES tkp = {0};
	//��ȡ���̷������Ƶľ��
	if ( !OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken ) )
	{
		return FALSE;
	}

	//�����鿴ϵͳȨ�޵���Ȩֵ��������Ϣ��һ��LUID�ṹ���
	//BOOL LookupPrivilegeValue(LPCTSTR lpSystemName,LPCTSTR lpName,PLUID lpLuid); ����
	//��һ��������ʾ��Ҫ�鿴��ϵͳ������ϵͳֱ����NULL ����
	//�ڶ���������ʾ��Ҫ�鿴����Ȩ��Ϣ�����ƣ�������winnt.h�У�����ָ��MSDN������windows nt privileges�� ����
	//�����������������������ص��ƶ���Ȩ���Ƶ���Ϣ�� ����
	//�������óɹ�����Ϣ�������������ΪLUID�Ľṹ���У����Һ������ط�0��
	if ( !LookupPrivilegeValue( NULL, szPrivilege, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//���û��ֹ ָ���������Ƶ���Ȩ
	if ( !AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////
// �������� :�Զ��幤�ߺ���
// ����ģ�� : 
////////////////////////////////////////////////////////////////
// ���� :ͨ��ָ���������õ������ ID
// ע�� : 
/////////////////////////////////////////////////////////////////
// ���� : sinister
// �����汾 : 1.00.00
// �������� : 2006.2.09
/////////////////////////////////////////////////////////////////
// ��   ��   ��   ��   ��   ʷ
////////////////////////////////////////////////////////////////
// �޸��� :
// �޸����� :
// �޸����� :
/////////////////////////////////////////////////////////////////

DWORD GetProcessId( LPCTSTR szProcName )
{
	PROCESSENTRY32 pe;  
	DWORD dwPid;
	DWORD dwRet;
	BOOL bFound = FALSE;

	//
	// ͨ�� TOOHLP32 ����ö�ٽ���
	//

	HANDLE hSP = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( hSP )
	{
		pe.dwSize = sizeof( pe );

		for ( dwRet = Process32First( hSP, &pe );
			dwRet;
			dwRet = Process32Next( hSP, &pe ) )
		{
			//
			// ʹ�� StrCmpNI �Ƚ��ַ������ɺ��Դ�Сд
			//
			if ( StrCmpNI( szProcName, pe.szExeFile, _tcslen( szProcName ) ) == 0 )
			{
				dwPid = pe.th32ProcessID;
				bFound = TRUE;
				break;
			}
		}

		CloseHandle( hSP );

		if ( bFound == TRUE )
		{
			return dwPid;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////
// �������� :�Զ��幤�ߺ���
// ����ģ�� : 
////////////////////////////////////////////////////////////////
// ���� : �������и�Ȩ�޵Ľ���
// ע�� :
/////////////////////////////////////////////////////////////////
// ���� : sinister
// �����汾 : 1.00.00
// �������� : 2006.2.09
/////////////////////////////////////////////////////////////////
// ��   ��   ��   ��   ��   ʷ
////////////////////////////////////////////////////////////////
// �޸��� :
// �޸����� :
// �޸����� :
/////////////////////////////////////////////////////////////////
//LPTSTR szProcessName������, BOOL bIsSystemPrivilege�Ƿ����ϵͳȨ��,TRUEΪSYSTEMȨ�ޣ�FALSEΪ����ԱȨ��
BOOL CreateProcessEx( LPTSTR szProcessName, BOOL bIsSystemPrivilege )
{	/*LPCTSTR;  LPCWSTR*/
	HANDLE hProcess;
	HANDLE hToken, hNewToken;
	DWORD dwPid;

	PACL pOldDAcl = NULL;
	PACL pNewDAcl = NULL;
	BOOL bDAcl;
	BOOL bDefDAcl;
	DWORD dwRet;

	PACL pSacl = NULL;
	PSID pSidOwner = NULL;
	PSID pSidPrimary = NULL;
	DWORD dwAclSize = 0;
	DWORD dwSaclSize = 0;
	DWORD dwSidOwnLen = 0;
	DWORD dwSidPrimLen = 0;

	DWORD dwSDLen;
	EXPLICIT_ACCESS ea;
	PSECURITY_DESCRIPTOR pOrigSd = NULL;
	PSECURITY_DESCRIPTOR pNewSd = NULL;

	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};

	BOOL bError;

	//������ǰ����Ȩ��
	if ( !EnableDebugPriv( _T("SeDebugPrivilege") ) )
	{
		printf( "EnableDebugPriv() to failed!\n" );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// ѡ�� WINLOGON ����  SYSTEMȨ��
	// ѡ�� Explorer.EXE ����  AdministratorȨ��
	//
	if (bIsSystemPrivilege)
	{
		dwPid = GetProcessId( _T("WINLOGON.EXE") );
	}
	else
	{
		dwPid = GetProcessId( _T("EXPLORER.EXE") );
	}

	if ( dwPid == NULL )
	{
		printf( "GetProcessId() to failed!\n" );   

		bError = TRUE;
		goto Cleanup;
	}

	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, dwPid );
	if ( hProcess == NULL )
	{
		printf( "OpenProcess() = %d\n", GetLastError() );   

		bError = TRUE;
		goto Cleanup;
	}

	if ( !OpenProcessToken( hProcess, READ_CONTROL | WRITE_DAC, &hToken ) )
	{
		printf( "OpenProcessToken() = %d\n", GetLastError() );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// ���� ACE �������з���Ȩ��
	//
	ZeroMemory( &ea, sizeof( EXPLICIT_ACCESS ) );
	BuildExplicitAccessWithName( &ea,
		_T("Everyone"),
		TOKEN_ALL_ACCESS,
		GRANT_ACCESS,
		0 );

	if ( !GetKernelObjectSecurity( hToken,
		DACL_SECURITY_INFORMATION,
		pOrigSd,
		0,
		&dwSDLen ) )
	{
		//
		// ��һ�ε��ø����Ĳ����϶��������������������Ŀ����
		// Ϊ�˵õ�ԭ��ȫ������ pOrigSd �ĳ���
		//
		if ( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
		{
			pOrigSd = ( PSECURITY_DESCRIPTOR ) HeapAlloc( GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				dwSDLen );
			if ( pOrigSd == NULL )
			{
				printf( "Allocate pSd memory to failed!\n" );

				bError = TRUE;
				goto Cleanup;
			}

			//
			// �ٴε��ò���ȷ�õ���ȫ������ pOrigSd
			//
			if ( !GetKernelObjectSecurity( hToken,
				DACL_SECURITY_INFORMATION,
				pOrigSd,
				dwSDLen,
				&dwSDLen ) )
			{
				printf( "GetKernelObjectSecurity() = %d\n", GetLastError() );
				bError = TRUE;
				goto Cleanup;
			}
		}
		else
		{
			printf( "GetKernelObjectSecurity() = %d\n", GetLastError() );
			bError = TRUE;
			goto Cleanup;
		}
	}

	//
	// �õ�ԭ��ȫ�������ķ��ʿ����б� ACL
	//
	if ( !GetSecurityDescriptorDacl( pOrigSd, &bDAcl, &pOldDAcl, &bDefDAcl ) )
	{
		printf( "GetSecurityDescriptorDacl() = %d\n", GetLastError() );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// ������ ACE Ȩ�޵ķ��ʿ����б� ACL
	//
	dwRet = SetEntriesInAcl( 1, &ea, pOldDAcl, &pNewDAcl ); 
	if ( dwRet != ERROR_SUCCESS )
	{
		printf( "SetEntriesInAcl() = %d\n", GetLastError() ); 
		pNewDAcl = NULL;

		bError = TRUE;
		goto Cleanup;
	} 

	if ( !MakeAbsoluteSD( pOrigSd,
		pNewSd,
		&dwSDLen,
		pOldDAcl,
		&dwAclSize,
		pSacl,
		&dwSaclSize,
		pSidOwner,
		&dwSidOwnLen,
		pSidPrimary,
		&dwSidPrimLen ) )
	{
		//
		// ��һ�ε��ø����Ĳ����϶��������������������Ŀ����
		// Ϊ�˴����µİ�ȫ������ pNewSd ���õ�����ĳ���
		//
		if ( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
		{
			pOldDAcl = ( PACL ) HeapAlloc( GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				dwAclSize );
			pSacl = ( PACL ) HeapAlloc( GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				dwSaclSize );
			pSidOwner = ( PSID ) HeapAlloc( GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				dwSidOwnLen );
			pSidPrimary = ( PSID ) HeapAlloc( GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				dwSidPrimLen );
			pNewSd = ( PSECURITY_DESCRIPTOR ) HeapAlloc( GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				dwSDLen );

			if ( pOldDAcl == NULL ||
				pSacl == NULL ||
				pSidOwner == NULL ||
				pSidPrimary == NULL ||
				pNewSd == NULL )
			{
				printf( "Allocate SID or ACL to failed!\n" );

				bError = TRUE;
				goto Cleanup;
			}

			//
			// �ٴε��òſ��Գɹ������µİ�ȫ������ pNewSd
			// ���µİ�ȫ��������Ȼ��ԭ���ʿ����б� ACL
			//
			if ( !MakeAbsoluteSD( pOrigSd,
				pNewSd,
				&dwSDLen,
				pOldDAcl,
				&dwAclSize,
				pSacl,
				&dwSaclSize,
				pSidOwner,
				&dwSidOwnLen,
				pSidPrimary,
				&dwSidPrimLen ) )
			{
				printf( "MakeAbsoluteSD() = %d\n", GetLastError() );

				bError = TRUE;
				goto Cleanup;
			}
		}
		else
		{
			printf( "MakeAbsoluteSD() = %d\n", GetLastError() );

			bError = TRUE;
			goto Cleanup;
		}
	}

	//
	// ���������з���Ȩ�޵ķ��ʿ����б� pNewDAcl ���뵽�µ�
	// ��ȫ������ pNewSd ��
	//
	if ( !SetSecurityDescriptorDacl( pNewSd, bDAcl, pNewDAcl, bDefDAcl ) )
	{
		printf( "SetSecurityDescriptorDacl() = %d\n", GetLastError() );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// ���µİ�ȫ�������ӵ� TOKEN ��
	//
	if ( !SetKernelObjectSecurity( hToken, DACL_SECURITY_INFORMATION, pNewSd ) )
	{
		printf( "SetKernelObjectSecurity() = %d\n", GetLastError() );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// �ٴδ� WINLOGON ���̵� TOKEN����ʱ�Ѿ��������з���Ȩ��
	//
	if ( !OpenProcessToken( hProcess, TOKEN_ALL_ACCESS, &hToken ) )
	{
		printf( "OpenProcessToken() = %d\n", GetLastError() );   

		bError = TRUE;
		goto Cleanup;
	}

	//
	// ����һ�ݾ�����ͬ����Ȩ�޵� TOKEN
	//
	if ( !DuplicateTokenEx( hToken,
		TOKEN_ALL_ACCESS,
		NULL,
		SecurityImpersonation,
		TokenPrimary,
		&hNewToken ) )
	{
		printf( "DuplicateTokenEx() = %d\n", GetLastError() );   

		bError = TRUE;
		goto Cleanup;
	}


	ZeroMemory( &si, sizeof( STARTUPINFO ) );
	si.cb = sizeof( STARTUPINFO );
	si.wShowWindow = SW_SHOW;

	//
	// �������½�û��Ļ��������½��̻���ʾ
	// 1314 �ͻ�û���������Ȩ����
	//
	ImpersonateLoggedOnUser( hNewToken );


	//
	// ���ǽ�������Ҫ������Ȩ�޽��̣������л��û�
	// ����Ҳ��������������棬������ TOKEN �㹻
	//


	//
	// ���þ�������Ȩ�޵� TOKEN��������Ȩ�޽���
	//

	PVOID p1 = &si;
	PVOID p2 = &pi;

	if ( !CreateProcessAsUser(
		hNewToken,
		szProcessName,
		NULL,
		NULL,
		NULL,
		FALSE,
		/*NULL,*/ NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi ) )
	{
		printf( "CreateProcessAsUser() = %d\n", GetLastError() );   

		bError = TRUE;
		goto Cleanup;
	}

	bError = FALSE;

Cleanup:
	if ( pOrigSd )
	{
		HeapFree( GetProcessHeap(), 0, pOrigSd );
	}
	if ( pNewSd )
	{
		HeapFree( GetProcessHeap(), 0, pNewSd );
	}
	if ( pSidPrimary )
	{
		HeapFree( GetProcessHeap(), 0, pSidPrimary );
	}
	if ( pSidOwner )
	{
		HeapFree( GetProcessHeap(), 0, pSidOwner );
	}
	if ( pSacl )
	{
		HeapFree( GetProcessHeap(), 0, pSacl );
	}
	if ( pOldDAcl )
	{
		HeapFree( GetProcessHeap(), 0, pOldDAcl );
	}

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	CloseHandle( hToken );
	CloseHandle( hNewToken );
	CloseHandle( hProcess );

	if ( bError )
	{
		return FALSE;
	}

	return TRUE;
}

