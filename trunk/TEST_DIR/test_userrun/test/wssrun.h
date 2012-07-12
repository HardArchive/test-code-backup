//创建高权限进程 
//
//Author:  sinister
//Email:   sinister@whitecell.org
//Homepage:http://www.whitecell.org 
//Date:    2006-02-12

/*****************************************************************
文件名        : wssrun.c
描述          : 创建高权限进程
作者          : sinister
最后修改日期  : 2006.2.09

*****************************************************************/


// 
// 写这个初衷是为了让 Windows 任务管理器可以结束掉一些服务
// 和僵死进程，用 pslist/pskill 之类工具无法获得象任务管理
// 器那样丰富的信息，还得来回切换，麻烦的很。最初想写个驱动
// 监视任务管理器运行，使用 SYSTEM 进程 TOKEN 替换来达到目的。
// 后来觉得通用性不好，就改用了这种方法。此方法还可使 regedit 
// 查看、编辑 SAM 等注册表键，何乐而不为。
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
// 函数类型 :自定义工具函数
// 函数模块 : 
////////////////////////////////////////////////////////////////
// 功能 :提升当前进程权限
// 注意 :
/////////////////////////////////////////////////////////////////
// 作者 : sinister
// 发布版本 : 1.00.00
// 发布日期 : 2006.2.09
/////////////////////////////////////////////////////////////////
// 重   大   修   改   历   史
////////////////////////////////////////////////////////////////
// 修改者 :
// 修改日期 :
// 修改内容 :
/////////////////////////////////////////////////////////////////
//LPCTSTR szPrivilege  "SeDebugPrivilege" //调适程序级进程令牌
BOOL EnableDebugPriv( LPCTSTR szPrivilege )
{
	HANDLE hToken = NULL;
	LUID sedebugnameValue = {0};
	TOKEN_PRIVILEGES tkp = {0};
	//获取进程访问令牌的句柄
	if ( !OpenProcessToken( GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken ) )
	{
		return FALSE;
	}

	//函数查看系统权限的特权值，返回信息到一个LUID结构体里。
	//BOOL LookupPrivilegeValue(LPCTSTR lpSystemName,LPCTSTR lpName,PLUID lpLuid); 　　
	//第一个参数表示所要查看的系统，本地系统直接用NULL 　　
	//第二个参数表示所要查看的特权信息的名称，定义在winnt.h中，具体指请MSDN索引“windows nt privileges” 　　
	//第三个参数用来接收所返回的制定特权名称的信息。 　　
	//函数调用成功后，信息存入第三个类型为LUID的结构体中，并且函数返回非0。
	if ( !LookupPrivilegeValue( NULL, szPrivilege, &sedebugnameValue ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//启用或禁止 指定访问令牌的特权
	if ( !AdjustTokenPrivileges( hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL ) )
	{
		CloseHandle( hToken );
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////
// 函数类型 :自定义工具函数
// 函数模块 : 
////////////////////////////////////////////////////////////////
// 功能 :通过指定进程名得到其进程 ID
// 注意 : 
/////////////////////////////////////////////////////////////////
// 作者 : sinister
// 发布版本 : 1.00.00
// 发布日期 : 2006.2.09
/////////////////////////////////////////////////////////////////
// 重   大   修   改   历   史
////////////////////////////////////////////////////////////////
// 修改者 :
// 修改日期 :
// 修改内容 :
/////////////////////////////////////////////////////////////////

DWORD GetProcessId( LPCTSTR szProcName )
{
	PROCESSENTRY32 pe;  
	DWORD dwPid;
	DWORD dwRet;
	BOOL bFound = FALSE;

	//
	// 通过 TOOHLP32 函数枚举进程
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
			// 使用 StrCmpNI 比较字符传，可忽略大小写
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
// 函数类型 :自定义工具函数
// 函数模块 : 
////////////////////////////////////////////////////////////////
// 功能 : 创建具有高权限的进程
// 注意 :
/////////////////////////////////////////////////////////////////
// 作者 : sinister
// 发布版本 : 1.00.00
// 发布日期 : 2006.2.09
/////////////////////////////////////////////////////////////////
// 重   大   修   改   历   史
////////////////////////////////////////////////////////////////
// 修改者 :
// 修改日期 :
// 修改内容 :
/////////////////////////////////////////////////////////////////
//LPTSTR szProcessName进程名, BOOL bIsSystemPrivilege是否具有系统权限,TRUE为SYSTEM权限，FALSE为管理员权限
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

	//提升当前进程权限
	if ( !EnableDebugPriv( _T("SeDebugPrivilege") ) )
	{
		printf( "EnableDebugPriv() to failed!\n" );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// 选择 WINLOGON 进程  SYSTEM权限
	// 选择 Explorer.EXE 进程  Administrator权限
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
	// 设置 ACE 具有所有访问权限
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
		// 第一次调用给出的参数肯定返回这个错误，这样做的目的是
		// 为了得到原安全描述符 pOrigSd 的长度
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
			// 再次调用才正确得到安全描述符 pOrigSd
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
	// 得到原安全描述符的访问控制列表 ACL
	//
	if ( !GetSecurityDescriptorDacl( pOrigSd, &bDAcl, &pOldDAcl, &bDefDAcl ) )
	{
		printf( "GetSecurityDescriptorDacl() = %d\n", GetLastError() );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// 生成新 ACE 权限的访问控制列表 ACL
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
		// 第一次调用给出的参数肯定返回这个错误，这样做的目的是
		// 为了创建新的安全描述符 pNewSd 而得到各项的长度
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
			// 再次调用才可以成功创建新的安全描述符 pNewSd
			// 但新的安全描述符仍然是原访问控制列表 ACL
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
	// 将具有所有访问权限的访问控制列表 pNewDAcl 加入到新的
	// 安全描述符 pNewSd 中
	//
	if ( !SetSecurityDescriptorDacl( pNewSd, bDAcl, pNewDAcl, bDefDAcl ) )
	{
		printf( "SetSecurityDescriptorDacl() = %d\n", GetLastError() );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// 将新的安全描述符加到 TOKEN 中
	//
	if ( !SetKernelObjectSecurity( hToken, DACL_SECURITY_INFORMATION, pNewSd ) )
	{
		printf( "SetKernelObjectSecurity() = %d\n", GetLastError() );

		bError = TRUE;
		goto Cleanup;
	}

	//
	// 再次打开 WINLOGON 进程的 TOKEN，这时已经具有所有访问权限
	//
	if ( !OpenProcessToken( hProcess, TOKEN_ALL_ACCESS, &hToken ) )
	{
		printf( "OpenProcessToken() = %d\n", GetLastError() );   

		bError = TRUE;
		goto Cleanup;
	}

	//
	// 复制一份具有相同访问权限的 TOKEN
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
	// 不虚拟登陆用户的话，创建新进程会提示
	// 1314 客户没有所需的特权错误
	//
	ImpersonateLoggedOnUser( hNewToken );


	//
	// 我们仅仅是需要建立高权限进程，不用切换用户
	// 所以也无需设置相关桌面，有了新 TOKEN 足够
	//


	//
	// 利用具有所有权限的 TOKEN，创建高权限进程
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

