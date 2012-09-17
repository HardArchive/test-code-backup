//
//	Compile with no debugging included
//
#include <windows.h>

typedef VOID (__stdcall * PINJCODE)(PVOID);

BOOL InjectThread(PPROCESS_INFORMATION ppi, PINJCODE pCode, ULONG nCodeLen, PVOID pData, ULONG nDataLen);

typedef struct 
{
	FARPROC pfnMessageBox;
	TCHAR	szText[100];

} INJDATA;

VOID WINAPI InjProc(INJDATA *pInjData)
{
	pInjData->pfnMessageBox(0, pInjData->szText, pInjData->szText, MB_OK);
}

int main()
{
	STARTUPINFO			si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	INJDATA InjData;

	InjData.pfnMessageBox = MessageBoxA;
	strcpy(InjData.szText, "Hello");

	if(CreateProcess(0, "notepad.exe", 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		InjectThread(&pi, InjProc, 100, &InjData, sizeof(InjData));

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	return 0;
}
